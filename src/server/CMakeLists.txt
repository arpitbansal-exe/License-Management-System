cmake_minimum_required(VERSION 3.10)
project(LicenseServer)
message(STATUS "⚙️ License Server: Building!")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source Files 
set(SOURCES 
    main.cpp
    config.h
    license_manager.h
    license_manager.cpp

    tls_server.h
    tls_server.cpp

    client_handler.h
    client_handler.cpp

    license.h
    
    client.h
    client.cpp

    UI.h
    UI.cpp

    SERVER.h
    SERVER.cpp

    render.cpp
	render.h

	ui_windows.h
	ui_windows.cpp

    pzheader.h
    pzheader.cpp
    ${CMAKE_SOURCE_DIR}/src/common/base64.h
    ${CMAKE_SOURCE_DIR}/src/common/base64.cpp
)

# Create Executable
add_executable(license_server ${SOURCES})
message(STATUS "⚙️ License Server: Adding Executable!")

message(STATUS "⚙️ License Server: Adding Precompiled Headers!")
target_precompile_headers(license_server PRIVATE pzheader.h)

# Include Directories
target_include_directories(license_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/common
    ${CMAKE_SOURCE_DIR}/external/json/include
    ${GLFW_ROOT_DIR}/include
    ${IMGUI_ROOT_DIR}
    )

# Link with OpenSSL (from Parent)
message(STATUS "⚙️ License license: Linking OpenSSL! ")
target_link_libraries(license_server
                    logger
                    OpenSSL::Crypto 
                    OpenSSL::SSL
                    nlohmann_json::nlohmann_json
                    glfw
                    imgui
                    opengl32
                )

# Visual Studio Configuration
message(STATUS "⚙️ License license: Configuring VS setting! ")
set_target_properties(license_server PROPERTIES
    OUTPUT_NAME "LicenseServer"
    PROJECT_LABEL "License Server"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
message(STATUS "⚙️ License Server: Done!")


