// client_handler.cpp

#include "client_handler.h"
#include <openssl/ssl.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "logger.h"
#include <thread>
#include <chrono>
using namespace std::chrono;
using json = nlohmann::json;

ClientHandler::ClientHandler(TLSServer& server, LicenseManager& manager)
    : tls_server(server), licenseManager(manager) {
    std::thread heartBeatSendThread(&ClientHandler::sendHeartbeat, this);
    heartBeatSendThread.detach();
    std::thread heartBeatCheckThread(&ClientHandler::checkHeartbeats, this);
    heartBeatCheckThread.detach();
}

void ClientHandler::handledActionLicense(Client& client, json request)
{
    std::cout << "handledActionLicense() called.\n";
    std::string action = request.value("action", "");
    std::string licenseName = request.value("license_name", "");
    try {
        if (action == "request_license")
            handleLicenseRequest(client, licenseName);
        else if (action == "release_license")
            handleLicenseRelease(client, licenseName);
    }
    catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        json invalidJSON = { {"status", "error"}, {"message", "Invalid JSON"} };
        tls_server.sendRequest(client.getSSL(), invalidJSON.dump());
    }
}

void ClientHandler::handleLicenseRequest(Client& client, std::string licenseName) {
    json response = licenseManager.allocateLicense(licenseName);
    if (response["status"] == "success") {
        // Construct a License object from the JSON response.
        License allocatedLicense(
            response["license"]["name"],
            response["license"]["license_id"],
            response["license"]["expiry"],
            response["license"]["quantity"],
            response["license"]["key"]
        );

        // Add the license to the client.
        client.addLicense(allocatedLicense);
    }
    tls_server.sendRequest(client.getSSL(), response.dump());
}

void ClientHandler::handleLicenseRelease(Client& client, std::string licenseName) {
    json response = licenseManager.releaseLicense(licenseName);
    // Optionally remove the license from the client if needed.
    tls_server.sendRequest(client.getSSL(), response.dump());
}

void ClientHandler::handleClient(SSL* ssl) {
    // Create a shared pointer to a Client instance.
    auto clientPtr = std::make_shared<Client>(ssl);
    addClient(clientPtr); // Store the shared pointer in our clients map.

    std::string received_str = tls_server.receiveResponse(ssl);
    if (received_str.empty()) {
        std::cerr << "Error reading from client.\n";
        return;
    }
    try {
        json request = json::parse(received_str);
        std::string action = request.value("action", "");
        if (action == "heartbeat_response") {
            std::string clientKey = getClientKey(*clientPtr);
            updateHeartbeat(clientKey);
        }
        // Use the same Client instance (dereferenced shared_ptr) for handling actions.
        handledActionLicense(*clientPtr, request);
    }
    catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        tls_server.sendRequest(ssl, "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
    }
}

std::string ClientHandler::getClientKey(Client& client)  {
    return client.getClientIP() + ":" + client.getClientPort();
}

void ClientHandler::addClient(std::shared_ptr<Client> clientPtr) {
    std::string key = getClientKey(*clientPtr);
    clients.emplace(key, clientPtr);
    std::cout << "Client added: " << key << "\n";
}

void ClientHandler::removeClient(Client& client) {
    std::string key = getClientKey(client);
    auto it = clients.find(key);
    if (it != clients.end()) {
        clients.erase(it);
        std::cout << "Client removed: " << key << "\n";
    }
    else {
        std::cerr << "Client not found: " << key << "\n";
    }
}

void ClientHandler::getClientsWithLicenses(
    std::vector<std::pair<std::shared_ptr<Client>, std::vector<License>>>& result)
{
    for (auto& [key, clientPtr] : clients) {
        auto licenses = clientPtr->getLicenses();
        result.emplace_back(clientPtr, licenses);
    }
}
int ClientHandler::getClientCount() {
    return clients.size();

}

void ClientHandler::updateHeartbeat(const std::string& clientKey) {
    std::lock_guard<std::mutex> lock(clientHandlerMutex);
    if (clients.find(clientKey) != clients.end()) {
        clients[clientKey]->getLastHearbeat() = steady_clock::now();
    }
}
void ClientHandler::checkHeartbeats() {
    while (true) { // Keep checking continuously
        std::this_thread::sleep_for(std::chrono::seconds(10)); // Check every 10 seconds
        auto now = std::chrono::steady_clock::now();
        std::lock_guard<std::mutex> lock(clientHandlerMutex);
        LOG_INFO("checkHeartbeats");

        for (auto it = clients.begin(); it != clients.end();) {
            std::shared_ptr<Client> clientPtr = it->second; // Get shared_ptr<Client>
            if (!clientPtr) {
                ++it;
                continue; // Skip null clients
            }

            Client& client = *clientPtr; // Dereference shared_ptr to get Client object
            steady_clock::time_point defaultTime = steady_clock::time_point();
            if (client.getLastHearbeat() == steady_clock::time_point()) {
                updateHeartbeat(getClientKey(client));
            }


            auto duration = std::chrono::duration_cast<std::chrono::seconds>(
                now - client.getLastHearbeat()).count();

            if (duration > 10) {  // 10-second timeout
                std::cout << "[HEARTBEAT] Client " << it->first
                    << " timed out. Releasing licenses.\n";
                releaseClientLicenses(it->first);
                it = clients.erase(it);  // Remove disconnected client
            }
            else {
                ++it;
            }
        }

    }
}

void ClientHandler::releaseClientLicenses(const std::string& clientKey) {
    auto it = clients.find(clientKey);
    if (it != clients.end() && it->second) {
        // Access client's licenses from shared_ptr
        for (const auto& license : it->second->getLicenses()) {
            licenseManager.releaseLicense(license.getName());
        }
    }
}
void ClientHandler::sendHeartbeat() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Ping every 5 seconds
        std::lock_guard<std::mutex> lock(clientHandlerMutex);
        for (auto& [clientKey, clientPtr] : clients) {
            LOG_INFO("Sending heartbeat to ",clientKey);
            if (clientPtr && clientPtr->isActive()) {
                const std::string pingMessage = R"({"action": "heartbeat"})";
                tls_server.sendRequest(clientPtr->getSSL(), pingMessage);
            }
        }
    }
}
