#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "license_manager.h"
#include "client_handler.h"
void setupUI(GLFWwindow* window);
void renderUI(LicenseManager& manager, ClientHandler& clientHandler);
void cleanupUI();