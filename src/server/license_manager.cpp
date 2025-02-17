#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <algorithm>

#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

#include "license_manager.h"
#include "base64.h"
#include "logger.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
// Load Public Key
RSA* load_public_key(const std::string& filename) {
    FILE* file = nullptr;
    if (fopen_s(&file, filename.c_str(), "r") != 0 || !file) {
        LOG_ERROR("Cannot open public key file: ", filename);
        return nullptr;
    }

    EVP_PKEY* pkey = PEM_read_PUBKEY(file, nullptr, nullptr, nullptr);
    fclose(file);

    if (!pkey) {
        LOG_ERROR("Failed to read public key ");
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    RSA* rsa = EVP_PKEY_get1_RSA(pkey);
    EVP_PKEY_free(pkey);
    return rsa;
}

// Constructor
LicenseManager::LicenseManager(const std::string& publicKeyPath, const std::string& licenseFilePath) {
    rsa_public = load_public_key(publicKeyPath);
    if (loadLicenseFile(licenseFilePath)) {
        PrintLicenses();
    }
}

// Load License File
bool LicenseManager::loadLicenseFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open license file: " << filepath << "\n";
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return parseLicenseFile(buffer.str());
}

// Print Licenses
void LicenseManager::PrintLicenses() const {
    std::cout << std::left
        << std::setw(20) << "License Name"
        << std::setw(40) << "License ID"
        << std::setw(15) << "Expiry Date"
        << std::setw(10) << "Quantity"
        << std::setw(10) << "Status"
        << "\n";

    std::cout << std::string(95, '-') << "\n";

    for (const auto& license : getLicenses()) {
        std::cout << std::left
            << std::setw(20) << license.name
            << std::setw(40) << license.license_id
            << std::setw(15) << license.expiry
            << std::setw(10) << license.quantity
            << std::setw(10) << (isLicenseExpired(license.expiry) ? "EXPIRED" : "ACTIVE")
            << "\n";
    }
}

// Parse .lic File
bool LicenseManager::parseLicenseFile(const std::string& content) {
    std::istringstream stream(content);
    std::string line;
    licenses.clear();

    while (std::getline(stream, line)) {
        if (line.find("Hardware_ID=") == 0) {
            hardware_id = line.substr(12);
        }
        else if (!line.empty()) {
            License license;
            std::istringstream lineStream(line);
            std::string token;

            while (lineStream >> token) {
                auto pos = token.find('=');
                if (pos != std::string::npos) {
                    std::string key = token.substr(0, pos);
                    std::string value = token.substr(pos + 1);
                    if (key == "License_ID") license.license_id = value;
                    else if (key == "KEY") license.key = value;
                    else if (key == "Expiry") license.expiry = value;
                    else if (key == "Quantity") license.quantity = std::stoi(value);
                    else if (key == "Name") license.name = value;
                }
            }

            if (!license.license_id.empty() && !license.key.empty()) {
                if (verifyLicense(license)) {
                    licenses.push_back(license);
                }
            }
        }
    }
    return true;
}

// Verify License Signature
bool LicenseManager::verifyLicense(const License& license) const {
    std::string data = hardware_id + license.name + license.license_id + license.expiry + std::to_string(license.quantity);
    if (rsaVerifySignature(data, license.key)) {
        LOG_INFO("License Signature Verified: ", license.name);
        return true;
    }
    else {
        LOG_WARNING("License Signature Verification Failed: ", license.name);
        return false;
    }
}

// Check if License is Expired
bool LicenseManager::isLicenseExpired(const std::string& expiryDate) const {
    std::tm tm = {};
    std::istringstream ss(expiryDate);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    std::time_t expiryTime = std::mktime(&tm);
    std::time_t currentTime = std::time(nullptr);

    return currentTime > expiryTime;
}

// RSA Signature Verification
bool LicenseManager::rsaVerifySignature(const std::string& data, const std::string& signature) const {
    if (!rsa_public) {
        LOG_ERROR("Error: Public key not loaded.");
        return false;
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    std::string decodedSig = base64Decode(signature);

    int verifyResult = RSA_verify(
        NID_sha256,
        hash,
        SHA256_DIGEST_LENGTH,
        reinterpret_cast<const unsigned char*>(decodedSig.data()),
        decodedSig.size(),
        rsa_public
    );

    if (verifyResult == 1) {
        return true;
    }
    else {
        ERR_print_errors_fp(stderr);
        return false;
    }
}

// Allocate License (Returns JSON with License Data)
json LicenseManager::allocateLicense(const std::string& license_name) {
    std::lock_guard<std::mutex> lock(licenseMutex);
    for (const auto& license : licenses) {
        if (license.name == license_name && activeLicenses[license_name] < license.quantity) {
            activeLicenses[license_name]++;
            LOG_INFO("License allocated: ", license_name);

            // Return license details in JSON
            json licenseJson = {
                {"status", "success"},
                {"message", "License allocated"},
                {"license", {
                    {"name", license.name},
                    {"license_id", license.license_id},
                    {"expiry", license.expiry},
                    {"quantity", license.quantity},
                    {"key", license.key}
                }}
            };
            return licenseJson;
        }
    }

    json errorResponse = {
        {"status", "error"},
        {"message", "No available licenses"}
    };
    return errorResponse;
}

// Release License
json LicenseManager::releaseLicense(const std::string& license_name) {
    std::lock_guard<std::mutex> lock(licenseMutex);
    if (activeLicenses.find(license_name) != activeLicenses.end() && activeLicenses[license_name] > 0) {
        activeLicenses[license_name]--;
        LOG_INFO("License released: ", license_name);
        json licenseJson = {
                {"status", "success"},
                {"message", "License allocated"},
                {"license",license_name} 
            };
        return licenseJson;
    }
    LOG_WARNING("License release failed: ", license_name);
    json errorResponse = {
        {"status", "error"},
        {"message", "License release failed"}
    };
    return errorResponse;
}
int LicenseManager::getActiveUsage(const std::string& licenseName) {
    std::lock_guard<std::mutex> lock(licenseMutex);
    auto it = activeLicenses.find(licenseName);
    return (it != activeLicenses.end()) ? it->second : 0;
}
