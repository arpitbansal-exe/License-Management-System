extern "C" {
#include <openssl/applink.c>
}
#include "ui/UI.h"
#include "server/server.h"
#include "logger.h"
#include "server/config.h"

int main() {
	// Initialize logger
	LOGGING::Logger::getInstance().configure(SERVER_LOGS_PATH, LOGS_ENABLED);

	// Initialize the UI
	GLFWwindow* window = SERVER_UI::initializeUI();
	if (!window) {
		LOG_ERROR("Failed to initialize GLFW.");
		return -1;
	}

	SERVER_UI::runUI(window);


	return 0;
}
