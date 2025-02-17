#include <iostream>
#include <fstream>

#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
extern "C" {
#include <openssl/applink.c>
}

#include "rsa_sign.h"
#include "utils.h"
#include "base64.h"

void print_openssl_errors() {
    unsigned long errCode;
    while ((errCode = ERR_get_error())) {
        char errBuffer[256];
        ERR_error_string_n(errCode, errBuffer, sizeof(errBuffer));
        std::cerr << "OpenSSL Error: " << errBuffer << std::endl;
    }
}
RSA* load_private_key(const std::string& filename) {
    FILE* file;
    print_file_contents(filename);

    if (fopen_s(&file, filename.c_str(), "r") != 0 || !file) {
        std::cerr << "Error: Cannot open " << filename << "\n";
        return nullptr;
    }

    // Read private key as EVP_PKEY
    EVP_PKEY* pkey = PEM_read_PrivateKey(file, nullptr, nullptr, nullptr);
    fclose(file);

    if (!pkey) {
        std::cerr << "Error: Failed to read private key\n";
        print_openssl_errors();
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    // Convert EVP_PKEY to RSA
    RSA* rsa = EVP_PKEY_get1_RSA(pkey);
    EVP_PKEY_free(pkey);

    if (!rsa) {
        std::cerr << "Error: Failed to extract RSA key from EVP_PKEY\n";
        return nullptr;
    }

    return rsa;
}

std::string sign_data(RSA* rsa, const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)data.c_str(), data.size(), hash);

    unsigned char* signature = new unsigned char[RSA_size(rsa)];
    unsigned int sig_len;

    if (RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, signature, &sig_len, rsa) != 1) {
        delete[] signature;
        return "";
    }

    std::string signed_base64 = base64_encode(signature, sig_len);
    delete[] signature;
    return signed_base64;
}
