#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "server/license_manager.h"
#include "server/client_handler.h"
namespace SERVER_UI {
	void renderUI();
}