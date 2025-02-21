#pragma once

#include <GLFW/glfw3.h>
#include "server/license_manager.h"
#include "server/client_handler.h"

// UI Window dimensions
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

// Function declarations
namespace SERVER_UI {
    GLFWwindow* initializeUI();
    void runUI(GLFWwindow* window);
    void cleanupUI();
}