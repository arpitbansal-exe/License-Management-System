#ifndef LICENSE_CLIENT_H
#define LICENSE_CLIENT_H

#include "tls_client.h"
#include <string>

class LicenseClient {
public:
    LicenseClient(TLSClient& client);
    std::string get_license(const std::string& license_name);

    bool request_license(const std::string& license_name);
    std::string recieve_license  (const std::string& license_name);
    bool release_license(const std::string& license_name);
private:
    TLSClient& tlsClient;
};

#endif // LICENSE_CLIENT_H
