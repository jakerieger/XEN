//
// Created by jr on 2/12/24.
//

#include "GraphicsContext.h"

namespace Graphics {
    GL::TWindow g_Window;
    FSize g_WindowSize;
    const char* g_WindowTitle;

    float lastFrame;

    float GetFrameTime() {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        const float deltaTime   = currentFrame - lastFrame;
        lastFrame               = currentFrame;
        return deltaTime;
    }

    bool Initialize(const FSize& size, const char* title) {
        g_WindowSize  = size;
        g_WindowTitle = title;

        if (glfwInit() != GLFW_TRUE) {
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        g_Window = GL::TWindow(glfwCreateWindow(g_WindowSize.Width,
                                                g_WindowSize.Height,
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

        glViewport(0, 0, g_WindowSize.Width, g_WindowSize.Height);
        // Framebuffer callback
        // Key callback

        return true;
    }

    void Shutdown() {
        glfwDestroyWindow(GetWindow());
        glfwTerminate();
    }
}  // namespace Graphics