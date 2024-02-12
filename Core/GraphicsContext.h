//
// Created by jr on 2/12/24.
//

#pragma once
#include "Size.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GL {
    struct FDestroyWindow {
        void operator()(GLFWwindow* ptr) const noexcept {
            glfwDestroyWindow(ptr);
        }
    };

    using TWindow = std::unique_ptr<GLFWwindow, FDestroyWindow>;
}  // namespace GL

namespace Graphics {
    extern GL::TWindow g_Window;
    extern FSize g_WindowSize;
    extern const char* g_WindowTitle;

    inline GLFWwindow* GetWindow() {
        return g_Window.get();
    }

    float GetFrameTime();
    bool Initialize(const FSize& size, const char* title);
    void Shutdown();
}  // namespace Graphics