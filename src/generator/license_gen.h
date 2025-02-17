#pragma once

#include <string>
#include <vector>

struct License {
    std::string name;
    std::string license_id;
    std::string expiry;
    int quantity;
    std::string signature;
};

/*
Generates a UUID (Universally Unique Identifier) Version 4
@return std::string - A randomly generated UUID string
*/
std::string generate_uuid();

/*
Generates and saves the license file containing the hardware ID and multiple licenses
@param const std::string& hardware_id - The unique hardware identifier (e.g., MAC address)
@param const std::vector<License>& licenses - List of licenses with their details and signatures
@param const std::string& output_file - Path to save the generated license file
*/
void generate_license_file(const std::string& hardware_id, const std::vector<License>& licenses, const std::string& output_file);


/*
Loads the private RSA key, signs all licenses, and updates their signatures
@param std::vector<License>& licenses - List of licenses to sign (modified in-place)
@param const std::string& hardware_id - Unique hardware identifier (e.g., MAC address)
*/
void load_and_sign_licenses(std::vector<License>& licenses, const std::string& hardware_id);

