#pragma once
#include "license_manager.h"
#include "client_handler.h"
void showALL_LicensesWindow(LicenseManager& manager);
void showClients(ClientHandler& clientHandler);
void ServerInfoWindow(LicenseManager& manager, ClientHandler& clientHandler);
void showLicenseOverview(LicenseManager& licenseManager);