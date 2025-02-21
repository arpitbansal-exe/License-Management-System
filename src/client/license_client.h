#pragma once
#include <string>
#include <memory>
#include "license_client_exception.h"
// Forward declaration (hides implementation details)
class TLSClient;

class LicenseClient {
public:
    LicenseClient(const std::string& serverAddress, int serverPort);
    ~LicenseClient();

    bool get_license(const std::string& license_name);
    bool release_license(const std::string& license_name);

    bool verifyLicense();
private:
    std::unique_ptr<TLSClient> tlsClient;
};