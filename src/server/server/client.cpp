#include "pzheader.cpp"

#include "license.h" 
#include "client.h"
#include "logger.h"

namespace SERVER{
    Client::Client(SSL * ssl) {
        _ssl = ssl;
        CreateClient(ssl);
    }
    void Client::CreateClient(SSL * ssl) {
        int sock = SSL_get_fd(ssl);
        if (sock < 0) {
            std::cerr << "Failed to get socket from SSL.\n";
            return;
        }
        sockaddr_in addr;
        int addr_len = sizeof(addr);
        if (getpeername(sock, (sockaddr*)&addr, &addr_len) == 0) {
            // Convert IP address to string
            char ipStr[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &addr.sin_addr, ipStr, sizeof(ipStr))) {
                client_ip = ipStr;
                client_port = std::to_string(ntohs(addr.sin_port));
            }
            else {
                std::cerr << "Failed to convert IP address.\n";
            }

            // Perform Reverse DNS Lookup (Get Hostname)
            char hostname[NI_MAXHOST];
            if (getnameinfo((sockaddr*)&addr, sizeof(addr), hostname, NI_MAXHOST, nullptr, 0, 0) == 0) {
                client_name = hostname;
            }
            else {
                client_name = "Unknown";
            }
        }
        else {
            std::cerr << "getpeername() failed.\n";
        }
    }

    void Client::addLicense(const License & license) {
        licenses.push_back(license);
        LOG_INFO("Added License: ", license.name, " to Client: ", client_name);
    }
    void Client::removeLicense(const License & license) {

    }

    std::vector<License> Client::getLicenses() const {
        LOG_INFO("Returning Licenses for Client: ", client_name, " Count: ", licenses.size());
        return licenses;
    }
    std::string Client::getClientIP() const
    {
        return client_ip;
    }

    std::string Client::getClientPort() const
    {
        return client_port;
    }

    std::string Client::getClientName() const
    {
        return client_name;
    }
    SSL * Client::getSSL() const {
        return _ssl;
    }
    std::chrono::steady_clock::time_point SERVER::Client::getLastHearbeat() const {
        return lastHeartbeat;
    }
    void Client::setLastHeartBeatt(const std::chrono::steady_clock::time_point & time) {
        lastHeartbeat = time;
    }
    bool Client::isActive() const {
        using namespace std::chrono;
        auto now = steady_clock::now();
        auto duration = duration_cast<seconds>(now - lastHeartbeat).count();
        return duration <= 10;
}
}