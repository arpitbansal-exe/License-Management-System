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
    LicenseClient licenseClient(tlsclient);
    std::string required_license = "SOLID_3D_BASE";
    std::string response = licenseClient.get_license(required_license);

    tlsclient.closeConnection();
    return 0;
}
