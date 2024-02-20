//
// Created by jr on 2/12/24.
//

#pragma once
#include "STL.h"
#include "Size.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics {
    GLFWwindow* GetWindow();
    void ToggleWireframe();
    void ToggleFullscreen();
    void ToggleVsync();
    void MarkWindowForClose();
    FSize GetWindowSize();
    float GetWindowAspect();
    float GetFrameTime();
    float GetFrameRate();
    bool Initialize(const FSize& size, const char* title);
    void Shutdown();

    namespace Screen {
        inline FSize TopLeft() {
            const auto screenSize = GetWindowSize();
            return {0, screenSize.Height};
        }

        inline FSize TopRight() {
            const auto screenSize = GetWindowSize();
            return {screenSize.Width, screenSize.Height};
        }

        inline FSize BottomLeft() {
            return {0, 0};
        }

        inline FSize BottomRight() {
            const auto screenSize = GetWindowSize();
            return {screenSize.Width, 0};
        }
    }  // namespace Screen

    namespace GL {
        void UpdateWindowSize(int width, int height);

        inline void
        FramebufferCallback(GLFWwindow* window, int width, int height) {
            UpdateWindowSize(width, height);
            glViewport(0, 0, width, height);
        }

        struct FDestroyWindow {
            void operator()(GLFWwindow* ptr) const noexcept {
                glfwDestroyWindow(ptr);
            }
        };

        using TWindow = eastl::unique_ptr<GLFWwindow, FDestroyWindow>;
    }  // namespace GL
}  // namespace Graphics