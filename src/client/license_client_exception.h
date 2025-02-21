#ifndef LICENSE_CLIENT_EXCEPTION_H
#define LICENSE_CLIENT_EXCEPTION_H

#include <stdexcept>
#include <string>

class LicenseClientException : public std::runtime_error {
public:
    explicit LicenseClientException(const std::string& message)
        : std::runtime_error("LicenseClient Error: " + message) {}
};

#endif // LICENSE_CLIENT_EXCEPTION_H