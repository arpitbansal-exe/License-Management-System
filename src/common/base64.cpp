#include <iostream>
#include <fstream>
#include <vector>

#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#include "base64.h"

std::string base64_encode(const unsigned char* buffer, size_t length) {
    BIO* bio, * b64;
    BUF_MEM* buffer_ptr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // No newlines
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, buffer, (int)length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &buffer_ptr);

    std::string base64_data(buffer_ptr->data, buffer_ptr->length);
    BIO_free_all(bio);

    return base64_data;
}
std::string base64Decode(const std::string& input) {
    BIO* bio = BIO_new_mem_buf(input.data(), (int)input.size());
    BIO* b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    std::vector<char> buffer(input.size());
    int length = BIO_read(bio, buffer.data(), (int)buffer.size());
    BIO_free_all(bio);

    return std::string(buffer.data(), length);
}