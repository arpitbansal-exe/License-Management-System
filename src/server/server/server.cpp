#include "pzheader.h"
#include "server.h"
#include "logger.h"
#include "config.h"

namespace SERVER {

    bool isServerRunning = false;
    TLSServer* server = nullptr;
    LicenseManager* manager = nullptr;
    ClientHandler* clientHandler = nullptr;


    int SERVER_PORT = 8443;
    char SERVER_CERT_PATH[256] = "certs\\server.crt";
    char SERVER_KEY_PATH[256] = "certs\\server.key";
    char PUBLIC_LS_KEY_PATH[256] = "certs\\public.pem";
    char LICENSE_FILE_PATH[256] = "licenses\\license.lic";;

    bool CreateServer() {
        // Allocate objects dynamically and assign them to global pointers.
        server = new TLSServer(SERVER_PORT, SERVER_CERT_PATH, SERVER_KEY_PATH);
        manager = new LicenseManager(PUBLIC_LS_KEY_PATH, LICENSE_FILE_PATH);
        clientHandler = new ClientHandler(*server, *manager);

        // Initialize the server components.
        initializeServer(*server, *manager, *clientHandler);

        // Mark the server as running.
        // Start the server.
        runServer(*server, *manager, *clientHandler);
        isServerRunning = true;

        return true;  // Indicating successful creation and running of the server.
    }

    void initializeServer(TLSServer& server, LicenseManager& manager, ClientHandler& clientHandler) {
        if (!server.initialize()) {
            LOG_ERROR("Failed to initialize server components.");
            exit(-1);
        }
    }

    void runServer(TLSServer& server, LicenseManager& manager, ClientHandler& clientHandler) {
        std::thread serverThread(&TLSServer::start, &server, std::ref(manager), std::ref(clientHandler));
        serverThread.detach();  // Detach so that it runs independently
    }
}
