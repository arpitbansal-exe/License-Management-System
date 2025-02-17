cmake_minimum_required(VERSION 3.10)
project(Client)
message(STATUS "💻 Client: Building!")

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source Files 
set(SOURCES 
    main.cpp
    tls_client.h
    tls_client.cpp
    license_client.h
    license_client.cpp
    ${CMAKE_SOURCE_DIR}/src/common/base64.h
    ${CMAKE_SOURCE_DIR}/src/common/base64.cpp
)

# Create Executable
add_executable(client ${SOURCES})
message(STATUS "⚙️ Client: Adding Executable!")

# Include Directories
target_include_directories(client PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/common
    ${CMAKE_SOURCE_DIR}/external/json/include  # JSON headers
)

# Link Libraries
message(STATUS "⚙️ Client: Linking Dependencies!")
target_link_libraries(client 
    OpenSSL::Crypto 
    OpenSSL::SSL 
    ws2_32
    nlohmann_json::nlohmann_json # Link JSON
)

# Visual Studio Configuration
message(STATUS "⚙️ Client: Configuring Visual Studio settings!")
set_target_properties(client PROPERTIES
    OUTPUT_NAME "Client"
    PROJECT_LABEL "Client"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

message(STATUS "✅ Client: Build Configuration Complete! 🚀")
