//
// Created by jr on 2/12/24.
//

#pragma once
#include "Size.h"
#include "STL.h"
#include "Scene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IGameApp {
public:
    virtual ~IGameApp()    = default;
    virtual void Startup() = 0;
    virtual void Cleanup() = 0;

    void AddScene(const AScene& scene);
    void LoadScene(const string& name);

    AScene* GetActiveScene() {
        for (auto& scene : m_Scenes) {
            if (scene.GetActive()) {
                return &scene;
            }
        }
        return nullptr;
    }

    AScene* GetScene(const string& name) {
        for (auto& scene : m_Scenes) {
            if (scene.GetName() == name) {
                return &scene;
            }
        }
        return nullptr;
    }

protected:
    vector<AScene> m_Scenes;
};

namespace Application {
    void InitializeApp(IGameApp& app, const FSize& size, const char* title);
    bool UpdateApp(IGameApp& app);
    void RunApp(IGameApp& app);
}  // namespace Application