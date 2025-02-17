#include "tls_client.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

TLSClient::TLSClient(const std::string& serverAddress, int serverPort)
    : serverAddress_(serverAddress), serverPort_(serverPort), sslCtx_(nullptr), ssl_(nullptr), clientFd_(-1) {
}

TLSClient::~TLSClient() {
    closeConnection();
}

bool TLSClient::initialize() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return false;
    }
    if (!createSocket()) return false;
    if (!configureTLS()) return false;
    return true;
}

bool TLSClient::configureTLS() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    sslCtx_ = SSL_CTX_new(TLS_client_method());
    if (!sslCtx_) {
        std::cerr << "Error creating SSL context\n";
        ERR_print_errors_fp(stderr);
        return false;
    }
    return true;
}

bool TLSClient::createSocket() {
    clientFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFd_ == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return false;
    }
    return true;
}

bool TLSClient::connectToServer() {
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort_);
    inet_pton(AF_INET, serverAddress_.c_str(), &serverAddr.sin_addr);

    if (connect(clientFd_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        closesocket(clientFd_);
        return false;
    }

    ssl_ = SSL_new(sslCtx_);
    SSL_set_fd(ssl_, clientFd_);

    if (SSL_connect(ssl_) <= 0) {
        std::cerr << "TLS handshake failed\n";
        ERR_print_errors_fp(stderr);
        return false;
    }

    std::cout << "Connected to server with TLS\n";
    return true;
}

bool TLSClient::sendRequest(const std::string& request) {
    if (SSL_write(ssl_, request.c_str(), static_cast<int>(request.size())) <= 0) {
        std::cerr << "Error sending request\n";
        return false;
    }
    return true;
}

std::string TLSClient::receiveResponse() {
    char buffer[4096] = { 0 };
    int bytesRead = SSL_read(ssl_, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        std::cerr << "Error receiving response\n";
        return "";
    }
    return std::string(buffer, bytesRead);
}

void TLSClient::closeConnection() {
    if (ssl_) {
        int shutdownResult = SSL_shutdown(ssl_);
        if (shutdownResult == 0) {
            SSL_shutdown(ssl_); // Second call for bidirectional shutdown
        }
        SSL_free(ssl_);
        ssl_ = nullptr;
    }
    if (sslCtx_) {
        SSL_CTX_free(sslCtx_);
        sslCtx_ = nullptr;
    }
    if (clientFd_ != INVALID_SOCKET) {
        closesocket(clientFd_);
        clientFd_ = INVALID_SOCKET;
    }
}

