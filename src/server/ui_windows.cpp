#include "ui_windows.h"
#include "imgui.h"
#include "license.h"
#include "config.h"
#include "client.h"
#include "logger.h"
void ServerInfoWindow(LicenseManager& manager,ClientHandler& clientHandler) {
    if (ImGui::Begin("Server Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        // Style for labels
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 200, 255, 255));

        // File Paths Section
        ImGui::TextUnformatted("File Paths");
        ImGui::Separator();

        ImGui::PopStyleColor();
        ImGui::Indent(10.0f);

        // Display paths with copy buttons
        if (ImGui::Button("Copy##1")) ImGui::SetClipboardText(PUBLIC_KEY_PATH.c_str());
        ImGui::SameLine();
        ImGui::Text("Public Key: %s", PUBLIC_KEY_PATH.c_str());

        if (ImGui::Button("Copy##2")) ImGui::SetClipboardText(LICENSE_FILE_PATH.c_str());
        ImGui::SameLine();
        ImGui::Text("License File: %s", LICENSE_FILE_PATH.c_str());

        if (ImGui::Button("Copy##3")) ImGui::SetClipboardText(SERVER_CERT_PATH.c_str());
        ImGui::SameLine();
        ImGui::Text("Server Certificate: %s", SERVER_CERT_PATH.c_str());

        if (ImGui::Button("Copy##4")) ImGui::SetClipboardText(SERVER_KEY_PATH.c_str());
        ImGui::SameLine();
        ImGui::Text("Server Key: %s", SERVER_KEY_PATH.c_str());

        if (ImGui::Button("Copy##5")) ImGui::SetClipboardText(manager.getHardwareID().c_str());
        ImGui::SameLine();
        ImGui::Text("Hardware ID: %s", manager.getHardwareID().c_str());

        if (ImGui::Button("Copy##6")) ImGui::SetClipboardText(SERVER_LOGS_PATH.c_str());
        ImGui::SameLine();
        ImGui::Text("Logs Path: %s", SERVER_LOGS_PATH.c_str());

        ImGui::Unindent(10.0f);

        // Server Configuration Section
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 200, 255, 255));
        ImGui::TextUnformatted("Server Configuration");
        ImGui::Separator();
        ImGui::PopStyleColor();

        ImGui::Indent(10.0f);
        ImGui::Text("Port: %d", SERVER_PORT);

        // Status indicators
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 200, 255, 255));
        ImGui::TextUnformatted("Status");
        ImGui::Separator();
        ImGui::PopStyleColor();

        // Example status indicators - you can modify these based on actual server status
        ImGui::Indent(10.0f);
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Server: Running");
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "TLS: Active");
        int clientCount = clientHandler.getClientCount();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Connections: %d", clientCount);


        ImGui::Unindent(10.0f);

        // Add tooltips for additional information
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Current active connections to the server");
    }
    ImGui::End();
}

void showALL_LicensesWindow(LicenseManager& manager) {
    if (ImGui::Begin("ALL Licenses")) {
        ImGui::Text("Currently Active Licenses:");
        if (ImGui::BeginTable("LicensesTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Sortable)) {
            // Table headers
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("License ID");
            ImGui::TableSetupColumn("Expiry Date");
            ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_PreferSortDescending);
            ImGui::TableHeadersRow();

            // Table rows
            for (const auto& license : manager.getLicenses()) {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", license.name.c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", license.license_id.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", license.expiry.c_str());

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%d", license.quantity);
            }

            ImGui::EndTable();
        }

    }
    ImGui::End();

}
void showClients(ClientHandler& clientHandler) {
    // Make result static to persist across frames.
    static std::vector<std::pair<std::shared_ptr<Client>, std::vector<License>>> result;
    static size_t totalLicenses = 0;

    // Static flag to track if a refresh is needed.
    static bool refreshNeeded = true;

    // Refresh logic: if refresh is needed, repopulate the static 'result'
    if (refreshNeeded) {
        result.clear();
        LOG_INFO("Func Called");
        clientHandler.getClientsWithLicenses(result);  // Make sure this iterates over 'clients'
        LOG_INFO("Result", result.size());

        // Recalculate total licenses from all clients.
        totalLicenses = 0;
        for (const auto& [_, licenses] : result) {
            totalLicenses += licenses.size();
        }

        refreshNeeded = false; // Reset the flag after refresh.
    }

    if (ImGui::Begin("Connected Clients")) {

        // Refresh button to force update.
        if (ImGui::Button("Refresh")) {
            refreshNeeded = true;
        }

        ImGui::SameLine();
        ImGui::Text("Last updated: %.2f seconds ago", ImGui::GetTime());

        // Display the table.
        if (ImGui::BeginTable("ClientsTable", 4,
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_Reorderable |
            ImGuiTableFlags_Sortable |
            ImGuiTableFlags_ScrollY)) {

            // Setup columns.
            ImGui::TableSetupColumn("Client Name", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("IP Address");
            ImGui::TableSetupColumn("Port");
            ImGui::TableSetupColumn("Licenses", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            // Populate table with client data.
            for (auto& [client, licenses] : result) {
                ImGui::TableNextRow();

                // Client Name.
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", client->getClientName().empty() ? "Unknown" : client->getClientName().c_str());

                // IP Address.
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", client->getClientIP().c_str());

                // Port.
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", client->getClientPort().c_str());

                // Licenses.
                ImGui::TableSetColumnIndex(3);
                if (licenses.empty()) {
                    ImGui::TextDisabled("No licenses");
                }
                else {
                    std::string childName = "Licenses_" + client->getClientIP() + ":" + client->getClientPort();
                    ImGui::BeginChild(childName.c_str(), ImVec2(-1, ImGui::GetTextLineHeight() * 2), false,
                        ImGuiWindowFlags_HorizontalScrollbar);
                    bool first = true;
                    for (const auto& license : licenses) {
                        if (!first) {
                            ImGui::SameLine();
                            ImGui::Text("|");
                            ImGui::SameLine();
                        }
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 200, 255, 255));
                        ImGui::Text("%s", license.name.c_str());
                        ImGui::PopStyleColor();

                        if (ImGui::IsItemHovered()) {
                            ImGui::BeginTooltip();
                            ImGui::Text("License ID: %s", license.license_id.c_str());
                            ImGui::Text("Expiry: %s", license.expiry.c_str());
                            ImGui::EndTooltip();
                        }
                        first = false;
                    }
                    ImGui::EndChild();
                }
            }
            ImGui::EndTable();
        }

        // Display summary at the bottom.
        ImGui::Spacing();
        ImGui::Text("Total Connected Clients: %zu", result.size());
        ImGui::SameLine();
        ImGui::Text("| Total Active Licenses: %zu", totalLicenses);
    }
    ImGui::End();
}

void showLicenseOverview(LicenseManager& licenseManager) {
    if (ImGui::Begin("License Overview")) {
        // Create a table with 3 columns: License Name, Total Available, and Current Usage.
        if (ImGui::BeginTable("LicenseTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("License Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Total Available", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Current Usage", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            // Retrieve the list of licenses from the LicenseManager.
            auto licenses = licenseManager.getLicenses();
            for (const auto& license : licenses) {
                ImGui::TableNextRow();

                // Column 0: License Name
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", license.name.c_str());

                // Column 1: Total Available (quantity)
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", license.quantity);

                // Column 2: Current Usage. We assume LicenseManager provides getActiveUsage().
                ImGui::TableSetColumnIndex(2);
                int usage = licenseManager.getActiveUsage(license.name);  // <-- You need to implement this if not available.
                ImGui::Text("%d", usage);
            }

            ImGui::EndTable();
        }
    }
    ImGui::End();
}

