//
// Created by jr on 2/12/24.
//

#pragma once
#include "Color.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IGameApp {
public:
    virtual ~IGameApp()                      = default;
    virtual void Startup()                   = 0;
    virtual void Cleanup()                   = 0;
    virtual bool IsDone()                    = 0;
    virtual void Update(float deltaTime)     = 0;
    virtual void LateUpdate(float deltaTime) = 0;
    virtual void RenderScene()               = 0;
    virtual void RenderUI()                  = 0;
};

namespace Application {
    void InitializeApp(IGameApp& app,
                       const int& width,
                       const int& height,
                       const char* title);
    bool UpdateApp(IGameApp& app);
    void RunApp(IGameApp& app);
}  // namespace Application