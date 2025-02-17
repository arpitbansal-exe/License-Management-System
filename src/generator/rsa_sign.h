#pragma once

#include <string>
#include <openssl/rsa.h>

/*
Load and return Private RSA Key
@param const std::string& filename - Path to the private key file
@return RSA* - Pointer to the loaded RSA key, or nullptr on failure
*/
RSA* load_private_key(const std::string& filename);

/*
Signs the data with Private RSA Key
@param RSA* rsa - Pointer to the loaded RSA private key
@param const std::string& data - Data to be signed
@return std::string - The generated digital signature as a Base64-encoded string
*/
std::string sign_data(RSA* rsa, const std::string& data);

