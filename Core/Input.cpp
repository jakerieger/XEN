//
// Created by jr on 2/12/24.
//

#include "Input.h"
#include "Interfaces/InputListener.h"

#define GLFW_KEY_NONE 0

namespace Input {
    vector<IInputListener*> g_Listeners;

    void KeyCallback(
      GLFWwindow*, const int key, int, const int action, const int mods) {
        const u32 _key  = static_cast<u32>(key);
        const u32 _mods = static_cast<u32>(mods);
        FKeyEvent event {_key, _mods};

        for (const auto listener : g_Listeners) {
            if (action == GLFW_PRESS) {
                listener->OnKeyDown(event);
            } else if (action == GLFW_RELEASE) {
                listener->OnKeyUp(event);
            }
        }
    }

    void
    MouseCallback(GLFWwindow*, const int button, const int action, const int) {
        const u32 _button = static_cast<u32>(button);
        FMouseEvent event {_button};

        for (const auto listener : g_Listeners) {
            if (action == GLFW_PRESS) {
                listener->OnMouseDown(event);
            } else if (action == GLFW_RELEASE) {
                listener->OnMouseUp(event);
            }
        }
    }

    void
    ScrollCallback(GLFWwindow*, const double xOffset, const double yOffset) {
        FScrollEvent event {xOffset, yOffset};

        for (const auto listener : g_Listeners) {
            listener->OnScroll(event);
        }
    }

    void Initialize(GLFWwindow* window) {
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetMouseButtonCallback(window, MouseCallback);
        glfwSetScrollCallback(window, ScrollCallback);
    }

    void RegisterListener(IInputListener* listener) {
        g_Listeners.push_back(listener);
    }

}  // namespace Input