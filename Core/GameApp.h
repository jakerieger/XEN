//
// Created by jr on 2/12/24.
//

#pragma once
#include "Size.h"
#include "STL.h"
#include "Scene.h"
#include "Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IGameApp : public IInputListener {
public:
    IGameApp() {
        Input::RegisterListener(this);
    }
    virtual void Startup() = 0;
    virtual void Cleanup() = 0;

    void AddScene(eastl::unique_ptr<AScene>& scene);
    void LoadScene(const eastl::string& name);

    AScene* GetActiveScene() {
        for (const auto& scene : m_Scenes) {
            if (scene->GetActive()) {
                return scene.get();
            }
        }
        return nullptr;
    }

    AScene* GetScene(const eastl::string& name) {
        for (const auto& scene : m_Scenes) {
            if (scene->GetName() == name) {
                return scene.get();
            }
        }
        return nullptr;
    }

protected:
    eastl::vector<eastl::unique_ptr<AScene>> m_Scenes;
};

namespace Application {
    void InitializeApp(IGameApp& app, const FSize& size, const char* title);
    bool UpdateApp(IGameApp& app);
    void RunApp(IGameApp& app);
}  // namespace Application