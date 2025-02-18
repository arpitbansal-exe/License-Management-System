#include "pzheader.h"
#include "tls_server.h"

#include "client_handler.h"
#include "license_manager.h"
#include "logger.h"
namespace SERVER {
	TLSServer::TLSServer(int port, const std::string& certFile, const std::string& keyFile)
		: port_(port), certFile_(certFile), keyFile_(keyFile), sslCtx_(nullptr), serverFd_(INVALID_SOCKET) {
	}

	TLSServer::~TLSServer() {
		if (serverFd_ != INVALID_SOCKET) {
			closesocket(serverFd_);
		}
		if (sslCtx_) {
			SSL_CTX_free(sslCtx_);
		}
		WSACleanup();
	}

	bool TLSServer::initialize() {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cerr << "WSAStartup failed\n";
			LOG_ERROR("WSAStartup failed");
			return false;
		}

		if (!createSocket()) {
			LOG_ERROR("Socket creation failed");
			return false;
		}

		if (!configureTLS()) {
			LOG_ERROR("TLS configuration failed");
			return false;
		}

		LOG_INFO("Server initialized on port ", port_);
		return true;
	}

	bool TLSServer::createSocket() {
		serverFd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (serverFd_ == INVALID_SOCKET) {
			LOG_ERROR("Socket creation failed");
			return false;
		}

		sockaddr_in serverAddr{};
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port_);
		serverAddr.sin_addr.s_addr = INADDR_ANY;

		if (bind(serverFd_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			LOG_ERROR("Bind failed");
			closesocket(serverFd_);
			return false;
		}

		if (listen(serverFd_, SOMAXCONN) == SOCKET_ERROR) {
			LOG_ERROR("Listen failed");
			closesocket(serverFd_);
			return false;
		}
		LOG_INFO("Socket ready on port ", port_);
		return true;
	}

	bool TLSServer::configureTLS() {
		// Initialize OpenSSL library
		SSL_load_error_strings();
		OpenSSL_add_ssl_algorithms();

		// Use TLS server method
		const SSL_METHOD* method = TLS_server_method();
		sslCtx_ = SSL_CTX_new(method);
		if (!sslCtx_) {
			LOG_ERROR("Failed to create SSL context");
			ERR_print_errors_fp(stderr);
			return false;
		}

		// Load server certificate
		if (SSL_CTX_use_certificate_file(sslCtx_, certFile_.c_str(), SSL_FILETYPE_PEM) <= 0) {
			LOG_ERROR("Failed to load server certificate");
			ERR_print_errors_fp(stderr);
			return false;
		}

		// Load private key
		if (SSL_CTX_use_PrivateKey_file(sslCtx_, keyFile_.c_str(), SSL_FILETYPE_PEM) <= 0) {
			LOG_ERROR("Failed to load server private key");
			ERR_print_errors_fp(stderr);
			return false;
		}

		if (!SSL_CTX_check_private_key(sslCtx_)) {
			LOG_ERROR("Private key does not match the certificate");
			return false;
		}
		LOG_INFO("TLS configuration completed.");
		return true;
	}

	void TLSServer::start(LicenseManager& manager, ClientHandler& client_handler) {
		LOG_INFO("Starting TLS Server on port ", port_);
		while (true) {
			sockaddr_in clientAddr{};
			int clientLen = sizeof(clientAddr);
			SOCKET clientFd = accept(serverFd_, (sockaddr*)&clientAddr, &clientLen);
			if (clientFd == INVALID_SOCKET) {
				LOG_WARNING("Accept failed");
				continue;
			}

			SSL* ssl = SSL_new(sslCtx_);
			SSL_set_fd(ssl, clientFd);

			if (SSL_accept(ssl) <= 0) {
				LOG_WARNING("TLS handshake failed");
				ERR_print_errors_fp(stderr);
				closesocket(clientFd);
				SSL_free(ssl);
				continue;
			}
			LOG_INFO("Client connected with TLS");
			std::thread clientThread(&ClientHandler::handleClient, &client_handler, ssl);
			clientThread.detach(); // Detach the thread so it runs independently
		}
	}

	bool TLSServer::sendRequest(SSL* ssl, const std::string& request) {
		if (SSL_write(ssl, request.c_str(), static_cast<int>(request.size())) <= 0) {
			LOG_ERROR("Error sending request");
			return false;
		}
		LOG_DEBUG("Sending: ", request);
		return true;
	}

	std::string TLSServer::receiveResponse(SSL* ssl) {
		char buffer[4096] = { 0 };
		int bytesRead = SSL_read(ssl, buffer, sizeof(buffer) - 1);
		if (bytesRead <= 0) {
			LOG_ERROR("Error receiving response");
			return "";
		}
		LOG_DEBUG("Recieved: ", std::string(buffer, bytesRead));
		return std::string(buffer, bytesRead);
	}
}