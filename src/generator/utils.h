#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>

/*
* Prints the content of file to console
* @param string& filename
* @return void
*/
void print_file_contents(const std::string& filename);
