# License Management System

A comprehensive license management solution consisting of three main components: a License Generator, License Server, and Client Library. This system provides secure license generation, validation, and management capabilities that can be integrated into any project.

## Components

### 1. License Generator
A standalone tool for creating and managing software licenses. Features include:
- Generate new licenses
- Secure license encryption using OpenSSL
- User-friendly ImGui interface (planned..)

### 2. License Server
A secure server application that handles license validation and management:
- TLS-encrypted communication
- Real-time license validation
- License status tracking and management
- Concurrent client connection handling

### 3. Client Library
A lightweight library that can be integrated into any project:
- Easy-to-use API for license validation
- Minimal dependencies
- Cross-platform compatibility
- Thread-safe operations
- Comprehensive error handling

## Dependencies

- **OpenSSL** (>=1.1.1) - For encryption and secure communication
- **nlohmann/json** (>=3.9.0) - For JSON parsing and configuration
- **Dear ImGui** - For the License Generator GUI
- **GLFW** (>=3.3) - Window management for the License Generator

## Building the Project

### Prerequisites
- CMake (>=3.15)
- C++17 compatible compiler

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/arpitbansal-exe/ls
cd ls
```
currently openssl needs to be built manually.

3. Generate build files:
```bash
mkdir build
cd build
cmake .. 
```

4. Build the project:
```bash
cmake --build .
```

## Security Features

- TLS encryption for all network communication
- License key encryption using industry-standard algorithms
- Protection against reverse engineering
- Tamper detection
- Hardware binding options


## Support

For bug reports and feature requests, please use the GitHub issue tracker.
For commercial support, contact: arpitbansal2304@gmail.com
