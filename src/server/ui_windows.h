#pragma once
#include "license_manager.h"
#include "client_handler.h"
namespace SERVER_UI {
	void StartupWindow();
	void showALL_LicensesWindow(LicenseManager& manager);
	void showClients(ClientHandler& clientHandler);
	void ServerInfoWindow(LicenseManager& manager, ClientHandler& clientHandler);
	void showLicenseOverview(LicenseManager& licenseManager);
}