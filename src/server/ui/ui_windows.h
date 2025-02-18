#pragma once
#include "server/license_manager.h"
#include "server/client_handler.h"
namespace SERVER_UI {
	void StartupWindow();
	void showALL_LicensesWindow(SERVER::LicenseManager& manager);
	void showClients(SERVER::ClientHandler& clientHandler);
	void ServerInfoWindow(SERVER::LicenseManager& manager, SERVER::ClientHandler& clientHandler);
	void showLicenseOverview(SERVER::LicenseManager& licenseManager);
}