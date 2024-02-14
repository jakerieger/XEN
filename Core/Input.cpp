//
// Created by jr on 2/12/24.
//

#include "glad/glad.h"
#include "Input.h"

#include <utility>
#define GLFW_KEY_NONE 0

namespace Input {
    struct FInputAction {
        explicit
        FInputAction(const int key, const int action, function<void()> callback)
            : Key(key), Action(action), Modifier(GLFW_KEY_NONE),
              Callback(std::move(callback)) {}
        explicit FInputAction(const int key,
                              const int action,
                              const int modifier,
                              function<void()> callback)
            : Key(key), Action(action), Modifier(modifier),
              Callback(std::move(callback)) {}

        int Key;
        int Action;
        int Modifier;
        function<void()> Callback;
    };

    vector<FInputAction> g_Actions;

    void KeyCallback(GLFWwindow* window,
                     const int key,
                     int scancode,
                     const int action,
                     const int mods) {
        for (auto& [Key, Action, Modifier, Callback] : g_Actions) {
            if (Key == key && Action == action && Modifier == mods) {
                Callback();
            }
        }
    }

    void Initialize(GLFWwindow* window) {
        glfwSetKeyCallback(window, KeyCallback);
    }

    void AddAction(const int key,
                   const int action,
                   const function<void()>& callback) {
        g_Actions.push_back(FInputAction {key, action, callback});
    }

    void AddAction(const int key,
                   const int action,
                   const int modifier,
                   const function<void()>& callback) {
        g_Actions.push_back(FInputAction {key, action, modifier, callback});
    }

}  // namespace Input