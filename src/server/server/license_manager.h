#ifndef LICENSE_MANAGER_H
#define LICENSE_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <openssl/rsa.h>
#include <mutex>

#include "license.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
// License Manager class
namespace SERVER {
    class LicenseManager {
    public:
        LicenseManager(const std::string& publicKeyPath, const std::string& licenseFilePath);
        bool loadLicenseFile(const std::string& filepath);
        bool verifyLicense(const License& license) const;
        bool isLicenseExpired(const std::string& expiryDate) const;
        void PrintLicenses() const;

        std::string getHardwareID() const { return hardware_id; }
        std::vector<License> getLicenses() const { return licenses; }

        json allocateLicense(const std::string& license_name);
        json releaseLicense(const std::string& license_name);

        int getActiveUsage(const std::string& licenseName);

    private:
        std::string hardware_id;
        std::vector<License> licenses;
        RSA* rsa_public;
        std::map<std::string, int> activeLicenses;
        std::mutex licenseMutex;
        bool parseLicenseFile(const std::string& content);
        bool rsaVerifySignature(const std::string& data, const std::string& signature) const;
    };
}
#endif // LICENSE_MANAGER_H
