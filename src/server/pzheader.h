// pcheader.h: Precompiled header file
#pragma once

// Standard libraries
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

// Networking
#include <ws2tcpip.h>

// OpenSSL libraries
#include <openssl/ssl.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>


// JSON Library
#include <nlohmann/json.hpp>

// ImGui Libraries
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
