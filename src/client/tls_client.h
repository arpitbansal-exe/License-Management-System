#ifndef TLS_CLIENT_H
#define TLS_CLIENT_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
class TLSClient {
public:
    TLSClient(const std::string& serverAddress, int serverPort);
    ~TLSClient();

    bool initialize();
    bool connectToServer();
    bool sendRequest(const std::string& request);
    std::string receiveResponse();
    void closeConnection();


private:
    std::string serverAddress_;
    int serverPort_;
    SSL_CTX* sslCtx_;
    SSL* ssl_;
    SOCKET clientFd_;

    bool configureTLS();
    bool createSocket();
};

#endif // TLS_CLIENT_H
