#include <iostream>
#include <thread>

// OpenSSL
extern "C" {
#include <openssl/applink.c>
}

// Project Headers
#include "logger.h"
#include "tls_server.h"
#include "license_manager.h"
#include "render.h"
#include "config.h"
#include "client_handler.h"
// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

// Function Declarations
void runServer(TLSServer& server,LicenseManager& manager, ClientHandler& clientHandler);
void runUI(GLFWwindow* window, LicenseManager& manager, ClientHandler& clientHandler);

// Main Function
int main() {
    LOGGING::Logger::getInstance().configure(SERVER_LOGS_PATH, LOGS_ENABLED);

    TLSServer server(SERVER_PORT, SERVER_CERT_PATH, SERVER_KEY_PATH);

    LicenseManager manager(PUBLIC_KEY_PATH, LICENSE_FILE_PATH);

    if (!server.initialize()) {
        LOG_ERROR("Failed to initialize TLS server.");
        return -1;
    }
    ClientHandler clientHandler(server, manager);

    std::thread serverThread(runServer, std::ref(server), std::ref(manager), std::ref(clientHandler));

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "License Server Dashboard", NULL, NULL);
    if (!window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    setupUI(window);


    runUI(window, manager,clientHandler);

    cleanupUI();
    glfwDestroyWindow(window);
    glfwTerminate();

    serverThread.join();

    return 0;
}

void runServer(TLSServer& server,LicenseManager& manager, ClientHandler& clientHandler) 
{
    server.start(manager, clientHandler);
}

void runUI(GLFWwindow* window, LicenseManager& manager,ClientHandler& clientHandler) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        renderUI(manager, clientHandler);
        glfwSwapBuffers(window);
    }
}
