#include "pzheader.h"
#include "UI.h"
#include "logger.h"

#include "render.h"
namespace SERVER_UI {
    GLFWwindow* initializeUI() {
        if (!glfwInit()) {
            LOG_ERROR("Failed to initialize GLFW");
            return nullptr;
        }

        // Set OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window
        GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "License Server Dashboard", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            return nullptr;
        }

        // Set OpenGL context for the window
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        // Enable docking
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport
        io.ConfigWindowsMoveFromTitleBarOnly = true;            // More reliable dragging

        // Set up ImGui style
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Initialize ImGui backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        return window;
    }

    void cleanupUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();  // Make sure GLFW is properly cleaned up
    }

    void runUI(GLFWwindow* window) {
        // Rendering loop
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            renderUI();
            glfwSwapBuffers(window);
        }
    }
}