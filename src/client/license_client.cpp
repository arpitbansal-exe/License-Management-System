#include "license_client.h"
#include "license_client_exception.h"
#include "tls_client.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

LicenseClient::LicenseClient(const std::string& serverAddress, int serverPort)
    : tlsClient(std::make_unique<TLSClient>(serverAddress, serverPort)) {

    if (!tlsClient->initialize()) {
        throw LicenseClientException("Failed to initialize TLS client.");
    }
    if (!tlsClient->connectToServer()) {
        throw LicenseClientException("Failed to connect to license server at " + serverAddress);
    }
}

LicenseClient::~LicenseClient() {
    tlsClient->closeConnection();
}

bool LicenseClient::get_license(const std::string& license_name) {
    json request = {
        {"action", "request_license"},
        {"license_name", license_name}
    };

    if (!tlsClient->sendRequest(request.dump())) {
        return "Error: Failed to send license request.";
    }

    std::string response = tlsClient->receiveResponse();
    if (response.empty()) {
        return "Error: No response from server.";
    }
    try {
        // Parse the JSON string
        json parsedJson = json::parse(response);

        // Extract status and message
        std::string status = parsedJson["status"];
        std::string message = parsedJson["message"];
        if (status == "success") 
        {
            std::string hardware_id = parsedJson["hardware_id"];
            json license = parsedJson["license"];
            std::string license_id = license["license_id"];
            std::string name = license["name"];
            std::string expiry = license["expiry"];
            std::string key = license["key"];
            int quantity = license["quantity"];

            std::cout << "Status: " << status << "\n";
            std::cout << "Message: " << message << "\n";
            std::cout << "HardwareID: " << hardware_id << "\n";
            
            std::cout << "License Name: " << name << "\n";
            std::cout << "License ID: " << license_id << "\n";
            std::cout << "Expiry Date: " << expiry << "\n";
            std::cout << "Key: " << key.substr(0, 20) << "... (truncated)\n"; // Truncate for readability
            std::cout << "Quantity: " << quantity << "\n";
            return true;
        }
    }
    catch (const json::exception& e) {
        std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
    }

    return false;
}

bool LicenseClient::release_license(const std::string& license_name) {
    json request = {
        {"action", "release_license"},
        {"license_name", license_name}
    };
    std::cout << "Release license request";
    if (!tlsClient->sendRequest(request.dump())) {
        throw LicenseClientException("Failed to release license: " + license_name);
    }
    std::string response = tlsClient->receiveResponse();
    if (response.empty()) {
        return "Error: No response from server.";
    }
    std::cout << "Resonse" << response;
    return true;
}
