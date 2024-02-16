//
// Created by jr on 2/16/24.
//

#pragma once

#include "SceneContext.h"

class AScene {
public:
    AScene() = default;

    void Initialize();
    void Update(float deltaTime);
    void Render();
    void LateUpdate(float deltaTime);
    void Destroy();

    void AddModel();
    void AddCamera();

    ADirectionalLight& GetSun() {
        return m_SceneContext.m_Sun;
    }

private:
    FSceneContext m_SceneContext;
};