#pragma once

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "license_manager.h"
#include "client_handler.h"
#pragma comment(lib, "ws2_32.lib")
namespace SERVER {
    class ClientHandler;
    class TLSServer {
    public:
        TLSServer(int port, const std::string& certFile, const std::string& keyFile);
        ~TLSServer();

        bool initialize();
        void start(LicenseManager& manager, ClientHandler& clientHandler);
        bool sendRequest(SSL* ssl, const std::string& request);
        std::string receiveResponse(SSL* ssl);
    private:
        int port_;
        std::string certFile_;
        std::string keyFile_;
        SSL_CTX* sslCtx_;
        SOCKET serverFd_;

        bool createSocket();
        bool configureTLS();
    };
}