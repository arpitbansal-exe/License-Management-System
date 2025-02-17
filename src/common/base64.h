#pragma once

#include <iostream>
#include <string>

/*
* Encode string in base64 for file writing
* @param const unsigned char* buffer, size_t length)
* @return std::string
*/
std::string base64_encode(const unsigned char* buffer, size_t length);
std::string base64Decode(const std::string& input);