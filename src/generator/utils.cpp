#include "utils.h"
#include <openssl/rsa.h>

void print_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        return;
    }

    std::cout << "Printing contents of " << filename << ":\n";
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";  // Print each line
    }
    file.close();
}
