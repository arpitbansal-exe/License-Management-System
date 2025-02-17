#include <iostream>
#include <vector>
#include "license_gen.h"
#include "mac_address.h"

int main() {
    // Step 1: Get Hardware ID
    std::string hardware_id = get_mac_address();

    // Step 2: Create Licenses
    std::vector<License> licenses = {
        {"SOLID_3D_BASE", generate_uuid(), "2026-01-01", 5, ""},
        {"ADVANCED_3D", generate_uuid(), "2027-05-15", 3, ""}
    };

    // Step 3: Sign Licenses (Delegated to generator)
    load_and_sign_licenses(licenses, hardware_id);

    // Step 4: Generate License File
    generate_license_file(hardware_id, licenses, "licenses\\license.lic");

    std::cout << "Licenses generated successfully.\n";
    return 0;
}
