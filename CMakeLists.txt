cmake_minimum_required(VERSION 3.10)
project(LicenseProvider)

message(STATUS "🚀 Starting License Provider Project Configuration...")

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message(STATUS "🛠️ C++ Standard set to 17.")

# ============================
# 📥 External Dependencies
# ============================
include(${CMAKE_SOURCE_DIR}/dependencies.cmake)

message(STATUS "📦 ImGui static library build to ${IMGUI_ROOT_DIR}")
# ============================
# 📁 Add Subdirectories
# ============================
message(STATUS "📁 Adding Generator Module...")
add_subdirectory(src/generator)

message(STATUS "📁 Adding Server Module...")
add_subdirectory(src/server)

message(STATUS "📁 Adding Client Module...")
add_subdirectory(src/client)
# Add Logger Module
message(STATUS "📁 Adding Logger Module...")
add_subdirectory(src/logger)

# ============================
# 🟢 Set Startup Project
# ============================
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT license_gen)
message(STATUS "✅ Startup Project set to license_gen")

message(STATUS "✅ License Provider Project Configuration Complete! 🚀")
