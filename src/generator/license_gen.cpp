// license_gen.cpp
#include "license_gen.h"
#include "mac_address.h"
#include "rsa_sign.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

// Function to generate a UUID v4 License_ID
std::string generate_uuid() {
    UUID uuid;
    UuidCreate(&uuid);
    char* str;
    UuidToStringA(&uuid, (RPC_CSTR*)&str);
    std::string uuid_str(str);
    RpcStringFreeA((RPC_CSTR*)&str);
    return uuid_str;
}

// Function to generate the license file
void generate_license_file(const std::string& hardware_id, const std::vector<License>& licenses, const std::string& output_file) {
    std::ofstream file(output_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << output_file << " for writing.\n";
        return;
    }

    file << "Hardware_ID=" << hardware_id << "\n\n";
    for (const auto& license : licenses) {
        file<< "Name=" << license.name << " "
            << "License_ID=" << license.license_id << " "
            << "KEY=" << license.signature << " "
            << "Expiry=" << license.expiry << " "
            << "Quantity=" << license.quantity << "\n";
    }

    file.close();
    std::cout << "License file generated: " << output_file << std::endl;
}

// Function to load RSA private key and sign licenses
void load_and_sign_licenses(std::vector<License>& licenses, const std::string& hardware_id) {
    // Load RSA private key
    RSA* rsa_private = load_private_key("certs\\private.pem");
    if (!rsa_private) {
        std::cerr << "Error: Failed to load private key!\n";
        return;
    }

    // Sign each license
    for (auto& license : licenses) {
        std::string data = hardware_id + license.name + license.license_id + license.expiry + std::to_string(license.quantity);
        license.signature = sign_data(rsa_private, data);  // Generate signature
    }

    // Free RSA private key
    RSA_free(rsa_private);
    std::cout << "Licenses signed successfully.\n";
}
