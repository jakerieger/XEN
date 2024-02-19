//
// Created by jr on 2/12/24.
//

#pragma once
#include "InputListener.h"
#include "STL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Input {
    void Initialize(GLFWwindow* window);
    void RegisterListener(IInputListener* listener);
}  // namespace Input