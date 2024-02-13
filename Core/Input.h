//
// Created by jr on 2/12/24.
//

#pragma once
#include "STL.h"
#include <GLFW/glfw3.h>

namespace Input {

    void Initialize(GLFWwindow* window);
    void AddAction(int key, int action, const function<void()>& callback);
    void AddAction(int key,
                   int action,
                   int modifier,
                   const function<void()>& callback);
}  // namespace Input