#pragma once

#include <openssl/ssl.h>
#include "tls_server.h"
#include <nlohmann/json.hpp>
#include "license_manager.h"
#include "client.h"
#include <string>
#include <unordered_map>
#include <set>
#include "client_handler.h"
#include <chrono>
namespace SERVER {
    using json = nlohmann::json;
    class TLSServer;
    class ClientHandler {
    public:
        ClientHandler(TLSServer& server, LicenseManager& manager);
        void handleClient(SSL* ssl);
        void handledActionLicense(Client& client, json request);

        void handleLicenseRequest(Client& client, std::string licenseName);
        void handleLicenseRelease(Client& client, std::string licenseName);

        void getClientsWithLicenses(std::vector<std::pair<std::shared_ptr<Client>, std::vector<License>>>& result);
        int getClientCount();

        //Heartbeat functionality
        void updateHeartbeat(const std::string& clientKey);
        void checkHeartbeats();
        void releaseClientLicenses(const std::string& clientKey);
    private:
        void addClient(std::shared_ptr<Client> clientPtr);
        void removeClient(const std::string& clientKey);
        std::string getClientKey(Client& client);

    private:
        TLSServer& tls_server;
        LicenseManager& licenseManager;
        std::mutex clientHandlerMutex;
        std::map<std::string, std::shared_ptr<Client>> clients;
    };
}