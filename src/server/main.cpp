//#include "pzheader.h"
extern "C" {
#include <openssl/applink.c>
}
#include "logger.h"
#include "config.h"
#include "UI.h"
#include "server.h"

int main() {
    // Initialize logger
    LOGGING::Logger::getInstance().configure(SERVER_LOGS_PATH, LOGS_ENABLED);

    // Initialize the UI
    GLFWwindow* window = SERVER_UI::initializeUI();
    if (!window) {
        LOG_ERROR("Failed to initialize GLFW.");
        return -1;
    }

    // Create the server and license manager, but don't start them yet
    //TLSServer server(SERVER_PORT, SERVER_CERT_PATH, SERVER_KEY_PATH);
    //LicenseManager manager(PUBLIC_KEY_PATH, LICENSE_FILE_PATH);
    //ClientHandler clientHandler(server, manager);

    // Run the UI (this loop continues until the window is closed)
    SERVER_UI::runUI(window);

    // Initialize and run the server once UI interaction is done

    // Run the server in a separate thread

    return 0;
}
