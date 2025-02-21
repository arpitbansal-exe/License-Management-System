#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "license.h"
#include <chrono>
namespace SERVER {

    class Client {
    public:
        Client(SSL* ssl);
        void CreateClient(SSL* ssl);
        void addLicense(const License& license);
        void removeLicense(const License& license);


        SSL* getSSL() const;
        std::string getClientIP()const;
        std::string getClientPort()const;
        std::string getClientName()const;
        std::vector<License> getLicenses() const;

        bool isActive() const;

        std::chrono::steady_clock::time_point getLastHearbeat() const;
        void setLastHeartBeatt(const std::chrono::steady_clock::time_point& time);
        Client(const Client& other)
            : _ssl(other._ssl),
            client_name(other.client_name),
            client_ip(other.client_ip),
            client_port(other.client_port),
            licenses(other.licenses) {
        }

        // Proper Copy Assignment Operator
        Client& operator=(const Client& other) {
            if (this != &other) {
                _ssl = other._ssl;
                client_name = other.client_name;
                client_ip = other.client_ip;
                client_port = other.client_port;
                licenses = other.licenses;
            }
            return *this;
        }
    private:
        SSL* _ssl;
        std::string client_name = "";
        std::string client_ip;
        std::string client_port;
        std::vector<License> licenses;
        std::chrono::steady_clock::time_point lastHeartbeat;
    };
}