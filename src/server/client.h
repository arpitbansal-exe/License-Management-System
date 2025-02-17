#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "license.h"
#include <chrono>

class Client {
private:
    SSL* _ssl;
    std::string client_name="";
    std::string client_ip;
    std::string client_port;
    std::vector<License> licenses;
    std::chrono::steady_clock::time_point lastHeartbeat;

public:
    Client(SSL* ssl);
    void CreateClient(SSL* ssl);
    void addLicense(const License& license);
    void removeLicense(const License& license);


    SSL* getSSL();
    std::string getClientIP();
    std::string getClientPort();
    std::string getClientName();
    
    bool isActive() const;

    std::vector<License> getLicenses() const;
    std::chrono::steady_clock::time_point getLastHearbeat();
    void Client::setLastHeartBeat(const std::chrono::steady_clock::time_point& time);
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
};
