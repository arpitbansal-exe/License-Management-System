// client_handler.cpp
#include "pzheader.h"

#include "client_handler.h"
#include "logger.h"

using namespace std::chrono;
using json = nlohmann::json;

ClientHandler::ClientHandler(TLSServer& server, LicenseManager& manager)
    : tls_server(server), licenseManager(manager) {
    //std::thread heartBeatCheckThread(&ClientHandler::checkHeartbeats, this);
    //heartBeatCheckThread.detach();
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
    tls_server.sendRequest(client.getSSL(), response.dump());
}

void ClientHandler::handleClient(SSL* ssl) {
    // Create a shared pointer to a Client instance.
    auto clientPtr = std::make_shared<Client>(ssl);
    std::string clientKey = getClientKey(*clientPtr);
    addClient(clientPtr); // Store the shared pointer in our clients map.

    while (true) {
        std::string received_str = tls_server.receiveResponse(ssl);
        if (received_str.empty()) {
            std::cerr << "Client disconnected.\n";
            releaseClientLicenses(clientKey);
            removeClient(clientKey); // Remove client from map if they disconnect
            break;
        }
        try {
            json request = json::parse(received_str);
            std::string action = request.value("action", "");

            if (action == "heartbeat") {
                updateHeartbeat(clientKey);
            }
            else {
                handledActionLicense(*clientPtr, request);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << "\n";
            tls_server.sendRequest(ssl, R"({"status":"error","message":"Invalid JSON"})");
        }
    }
    // Gracefully shutdown the SSL connection after all requests are processed
    SSL_shutdown(ssl);
    SSL_free(ssl);
    closesocket(SSL_get_fd(ssl));  // Close the socket
}



std::string ClientHandler::getClientKey(Client& client)  {
    return client.getClientIP() + ":" + client.getClientPort();
}

void ClientHandler::addClient(std::shared_ptr<Client> clientPtr) {
    std::string key = getClientKey(*clientPtr);
    clients.emplace(key, clientPtr);
    std::cout << "Client added: " << key << "\n";
}

void ClientHandler::removeClient(const std::string& clientKey) {
    std::lock_guard<std::mutex> lock(clientHandlerMutex); // Thread-safety
    auto it = clients.find(clientKey);
    if (it != clients.end()) {
        clients.erase(it);
        LOG_INFO("Client removed: ", clientKey);
    }
    else {
        LOG_WARNING("Client not found: ", clientKey);
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
    if (clients.find(clientKey) != clients.end()) {
        LOG_DEBUG( "Heartbeat updated for", clientKey);
        clients[clientKey]->setLastHeartBeat(steady_clock::now());
    }
}
void ClientHandler::checkHeartbeats() {
    while (true) { // Keep checking continuously
        std::this_thread::sleep_for(std::chrono::seconds(10)); // Check every 10 seconds
        auto now = std::chrono::steady_clock::now();
        std::lock_guard<std::mutex> lock(clientHandlerMutex);
        LOG_DEBUG("checkHeartbeats");

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
