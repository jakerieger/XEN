//
// Created by jr on 2/12/24.
//

#include "GraphicsContext.h"

namespace Graphics {
    GL::TWindow g_Window;
    FSize g_InitWindowSize;
    FSize g_CurrentWindowSize;
    const char* g_WindowTitle;
    bool g_IsWireframe = false;
    float g_LastFrame;
    bool g_IsFullscreen = false;

    GLFWwindow* GetWindow() {
        return g_Window.get();
    }

    float GetFrameTime() {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        const float deltaTime   = currentFrame - g_LastFrame;
        g_LastFrame             = currentFrame;
        return deltaTime;
    }

    void ToggleWireframe() {
        if (g_IsWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        g_IsWireframe = !g_IsWireframe;
    }

    void ToggleFullscreen() {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (g_IsFullscreen) {
            glfwSetWindowMonitor(GetWindow(),
                                 nullptr,
                                 0,
                                 0,
                                 g_InitWindowSize.Width,
                                 g_InitWindowSize.Height,
                                 mode->refreshRate);

            g_CurrentWindowSize = g_InitWindowSize;
        } else {
            glfwSetWindowMonitor(GetWindow(),
                                 glfwGetPrimaryMonitor(),
                                 0,
                                 0,
                                 mode->width,
                                 mode->height,
                                 mode->refreshRate);

            g_CurrentWindowSize = FSize(mode->width, mode->height);
        }

        g_IsFullscreen = !g_IsFullscreen;
    }

    void MarkWindowForClose() {
        glfwSetWindowShouldClose(GetWindow(), true);
    }

    FSize GetWindowSize() {
        return g_CurrentWindowSize;
    }

    bool Initialize(const FSize& size, const char* title) {
        g_InitWindowSize    = size;
        g_CurrentWindowSize = size;
        g_WindowTitle       = title;

        if (glfwInit() != GLFW_TRUE) {
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        g_Window = GL::TWindow(glfwCreateWindow(size.Width,
                                                size.Height,
                                                g_WindowTitle,
                                                nullptr,
                                                nullptr));
        if (g_Window == nullptr) {
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(g_Window.get());

        if (!gladLoadGLLoader(
              reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            return false;
        }

        glViewport(0, 0, size.Width, size.Height);
        glfwSetFramebufferSizeCallback(GetWindow(), GL::FramebufferCallback);

        return true;
    }

    void Shutdown() {
        glfwDestroyWindow(GetWindow());
        glfwTerminate();
    }

    namespace GL {
        void UpdateWindowSize(const int width, const int height) {
            g_CurrentWindowSize = FSize(width, height);
        }
    }  // namespace GL
}  // namespace Graphics