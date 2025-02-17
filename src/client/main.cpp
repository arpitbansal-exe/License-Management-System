#include "tls_client.h"
#include <iostream>
#include "license_client.h"

int main() {
    const std::string serverAddress = "127.0.0.1";
    const int serverPort = 8443;

    TLSClient tlsclient(serverAddress, serverPort);

    if (!tlsclient.initialize() || !tlsclient.connectToServer()) {
        std::cerr << "Failed to initialize or connect to the server.\n";
        return 1;
    }

    // Start heartbeat in a background thread
    tlsclient.startHeartbeat();

    // Simulate license request
    LicenseClient licenseClient(tlsclient);
    std::string required_license = "SOLID_3D_BASE";
    std::string response = licenseClient.get_license(required_license);
    std::cout << "License response: " << response << std::endl;

    // Allow client to run and send heartbeats for some time
    std::this_thread::sleep_for(std::chrono::minutes(5));

    // Stop heartbeat and close connection
    tlsclient.stopHeartbeat();
    tlsclient.closeConnection();

    return 0;
}
