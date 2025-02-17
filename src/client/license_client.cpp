#include "license_client.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

LicenseClient::LicenseClient(TLSClient& client) : tlsClient(client) {}

std::string LicenseClient::get_license(const std::string& license_name)
{
    if (request_license(license_name)) {
        std::cout << "Requested license for " << license_name << ".\n";
    }
    else {
        std::cout << "Requested license for " << license_name << "Failed.\n";
        return "";
    }
    return recieve_license (license_name);
}

bool LicenseClient::release_license(const std::string& license_name)
{
    json request = {
        {"action", "release_license"},
        {"license_name", license_name}
    };
    std::string requestStr = request.dump();
    return tlsClient.sendRequest(requestStr);
}

bool LicenseClient::request_license(const std::string& license_name) 
{
    json request = {
        {"action", "request_license"},
        {"license_name", license_name}
    };
    std::string requestStr = request.dump();

    return tlsClient.sendRequest(requestStr);
}

std::string LicenseClient::recieve_license(const std::string& license_name) 
{
    std::string response = tlsClient.receiveResponse();
    std::cout << "Recieved license" << response;
    if (response.empty()) {
        std::cerr << "No response from server.\n";
        return "";
    }
    return response;
}
