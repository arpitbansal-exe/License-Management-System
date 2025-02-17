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
include(FetchContent)
include(FetchContent)

# Set FetchContent base directory to external/
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

# Download nlohmann/json into /external
set(JSON_ROOT_DIR ${CMAKE_SOURCE_DIR}/external/json)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    SOURCE_DIR ${JSON_ROOT_DIR}
)
FetchContent_MakeAvailable(nlohmann_json)
message(STATUS "📦 JSON library downloaded to ${JSON_ROOT_DIR}")

# OpenSSL Configuration
set(OPENSSL_ROOT_DIR ${CMAKE_SOURCE_DIR}/external/openssl-build)
message(STATUS "🔑 OpenSSL Root Directory: ${OPENSSL_ROOT_DIR}")

find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    message(STATUS "✅ OpenSSL Found: Version ${OPENSSL_VERSION}")
else()
    message(FATAL_ERROR "❌ OpenSSL not found. Please check installation.")
endif()


# -------------------------
# 🖼️ GLFW (Windowing Library)
# -------------------------
set(GLFW_ROOT_DIR ${CMAKE_SOURCE_DIR}/external/glfw)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
    SOURCE_DIR ${GLFW_ROOT_DIR}
)
FetchContent_MakeAvailable(glfw)
message(STATUS "📦 GLFW downloaded to ${GLFW_ROOT_DIR}")

# ============================
# 🖼️ ImGui with Docking (Multi-Viewports)
# ============================
set(IMGUI_ROOT_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking  # Use docking branch for viewports support
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/imgui
)
FetchContent_MakeAvailable(imgui)
message(STATUS "🖼️ ImGui (Docking) downloaded to ${IMGUI_ROOT_DIR}")

set(IMGUI_SOURCES
    ${IMGUI_ROOT_DIR}/imgui.cpp
    ${IMGUI_ROOT_DIR}/imgui_draw.cpp
    ${IMGUI_ROOT_DIR}/imgui_widgets.cpp
    ${IMGUI_ROOT_DIR}/imgui_tables.cpp
    ${IMGUI_ROOT_DIR}/imgui_demo.cpp  # Optional: For testing
    ${IMGUI_ROOT_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_ROOT_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    ${IMGUI_ROOT_DIR}
    ${IMGUI_ROOT_DIR}/backends
    ${GLFW_ROOT_DIR}/include
)
# Enable Viewports and Docking
target_compile_definitions(imgui PUBLIC
    IMGUI_ENABLE_VIEWPORTS
    IMGUI_ENABLE_DOCKING
)
target_link_libraries(imgui PRIVATE glfw)

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
