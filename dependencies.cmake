# ============================
# 🚀 Dependencies.cmake
# ============================

include(FetchContent)

# Set FetchContent base directory to external/
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

# Enable solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 1️⃣ JSON Library (nlohmann/json)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/json
)
FetchContent_MakeAvailable(nlohmann_json)
message(STATUS "📦 JSON library downloaded.")

# 2️⃣ OpenSSL
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    message(STATUS "✅ OpenSSL Found: ${OPENSSL_VERSION}")
else()
    message(FATAL_ERROR "❌ OpenSSL not found!")
endif()

# 3️⃣ GLFW (Windowing Library)
# Disable all extra GLFW components
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_DOCUMENT_INTERNALS OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)
set(GLFW_UPDATE_MAPPINGS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/glfw
)
FetchContent_MakeAvailable(glfw)
message(STATUS "📦 GLFW downloaded.")

# Move GLFW targets to Dependencies folder
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
if(TARGET uninstall)
    set_target_properties(uninstall PROPERTIES FOLDER "Dependencies")
endif()
if(TARGET update_mappings)
    set_target_properties(update_mappings PROPERTIES FOLDER "Dependencies")
endif()

# 4️⃣ ImGui (Docking)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking  # Use the docking branch
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/imgui
)
FetchContent_MakeAvailable(imgui)
message(STATUS "🖼️ ImGui downloaded.")

# Build ImGui Library
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imgui/backends
)
target_link_libraries(imgui PRIVATE glfw)
target_compile_definitions(imgui PUBLIC IMGUI_ENABLE_VIEWPORTS IMGUI_ENABLE_DOCKING)
set_target_properties(imgui PROPERTIES FOLDER "Dependencies")
message(STATUS "✅ ImGui Library Configured.")

# ============================
# 🚀 Dependencies Loaded!
# ============================