//
// Created by jr on 2/12/24.
//

#pragma once
#include "STL.h"
#include "Size.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GL {
    inline void FramebufferCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    struct FDestroyWindow {
        void operator()(GLFWwindow* ptr) const noexcept {
            glfwDestroyWindow(ptr);
        }
    };

    using TWindow = unique_ptr<GLFWwindow, FDestroyWindow>;
}  // namespace GL

namespace Graphics {
    GLFWwindow* GetWindow();
    void ToggleWireframe();
    void ToggleFullscreen();
    void MarkWindowForClose();
    float GetFrameTime();
    bool Initialize(const FSize& size, const char* title);
    void Shutdown();
}  // namespace Graphics