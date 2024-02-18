//
// Created by jr on 2/16/24.
//

#pragma once

#include "SceneContext.h"
#include "Camera.h"
#include "STL.h"

class AScene final : public ILifetime {
public:
    explicit AScene(const string& name) : m_Name(name) {
        Awake();
    }

    void Awake() override;
    void Start() override;
    void Update(float deltaTime) override;
    void LateUpdate() override;
    void FixedUpdated() override;
    void Destroyed() override;

    void Render();

    void SetActive(const bool active) {
        m_Active = active;
    }

    bool GetActive() const {
        return m_Active;
    }

    string& GetName() {
        return m_Name;
    }

    ADirectionalLight& GetSun() {
        return m_SceneContext.m_Sun;
    }

    static ACamera* GetActiveCamera(FSceneContext& context);

    FSceneContext& GetContext() {
        return m_SceneContext;
    }

    template<typename T>
    void AddGameObject(T& gameObject) {
        static_assert(std::is_base_of_v<IGameObject, T>,
                      "T must be a subclass of IGameObject");
        m_SceneContext.m_GameObjects.push_back(&gameObject);
    }

private:
    FSceneContext m_SceneContext;
    string m_Name;
    bool m_Active = false;
};