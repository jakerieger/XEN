//
// Created by jr on 2/12/24.
//

#include "glad/glad.h"
#include "Input.h"

#include "InputListener.h"

#include <utility>
#define GLFW_KEY_NONE 0

namespace Input {
    vector<IInputListener*> g_Listeners;

    void KeyCallback(GLFWwindow* window,
                     const int key,
                     int scancode,
                     const int action,
                     const int mods) {
        FKeyEvent event;
        event.KeyCode   = key;
        event.Action    = action;
        event.Modifiers = mods;
        for (const auto& listener : g_Listeners) {
            listener->OnKeyDown(event);
        }
    }

    void Initialize(GLFWwindow* window) {
        glfwSetKeyCallback(window, KeyCallback);
    }

    void RegisterListener(IInputListener* listener) {
        g_Listeners.push_back(listener);
    }

}  // namespace Input