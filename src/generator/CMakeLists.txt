cmake_minimum_required(VERSION 3.10)
project(LicenseGenerator)

message(STATUS "🔨License Generator: Building")


# Source Files
set(SOURCES 
    main.cpp
    license_gen.h
    license_gen.cpp
    rsa_sign.h
    rsa_sign.cpp
    mac_address.cpp
    mac_address.h
    utils.h
    utils.cpp
    ${CMAKE_SOURCE_DIR}/src/common/base64.h
    ${CMAKE_SOURCE_DIR}/src/common/base64.cpp
)

# Create Executable
message(STATUS "🔨 Configuring License Generator...")
add_executable(license_gen ${SOURCES})
message(STATUS "✅ License Generator Executable Created.")

# Include Directories (Add src/common)
target_include_directories(license_gen PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/src/common
)

# Link with OpenSSL
target_link_libraries(license_gen OpenSSL::Crypto)

message(STATUS "🔨License Generator: Configuring VS setting!")

# Visual Studio Configuration
set_target_properties(license_gen PROPERTIES
    OUTPUT_NAME "LicenseGenerator"
    PROJECT_LABEL "License Generator"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
message(STATUS "🔨License Generator: Done!!!!")