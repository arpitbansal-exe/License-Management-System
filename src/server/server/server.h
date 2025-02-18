#pragma once
#include "tls_server.h"
#include "license_manager.h"
#include "client_handler.h"
#include <string>

namespace SERVER {
    extern bool isServerRunning;
    extern TLSServer* server;
    extern LicenseManager* manager;
    extern ClientHandler* clientHandler;

    // Default configuration values (can be customized before calling CreateServer)
    extern int SERVER_PORT;
    extern char SERVER_CERT_PATH[256];
    extern char SERVER_KEY_PATH[256];
    extern char PUBLIC_LS_KEY_PATH[256];
    extern char LICENSE_FILE_PATH[256];

    bool CreateServer();
    void initializeServer(TLSServer& server, LicenseManager& manager, ClientHandler& clientHandler);
    void runServer(TLSServer& server, LicenseManager& manager, ClientHandler& clientHandler);
}