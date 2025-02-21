#include "license_client.h"
#include <iostream>

int main() {
    try {
        LicenseClient licenseClient("127.0.0.1", 8443);

        if (licenseClient.get_license("SOLID_3D_BASE"))
            std::cout << "Success";

        licenseClient.release_license("SOLID_3D_BASE");
    }
    catch (const LicenseClientException& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
