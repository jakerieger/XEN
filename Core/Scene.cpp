//
// Created by jr on 2/16/24.
//

#include "Scene.h"

void AScene::Initialize() {
    m_SceneContext.m_Cameras = {};
    m_SceneContext.m_Models  = {};
    m_SceneContext.m_Sun.GetTransform().Translate(0.f, 0.f, -5.f);
}

void AScene::Update(float deltaTime) {
    for (auto& model : m_SceneContext.m_Models) {
        model.Update(deltaTime);
    }

    m_SceneContext.m_Sun.GetTransform().Update();
}

void AScene::Render() {
    for (auto& model : m_SceneContext.m_Models) {
        model.Draw(m_SceneContext);
    }
}

void AScene::LateUpdate(float deltaTime) {}

void AScene::Destroy() {}

void AScene::AddModel(const AModel& model) {
    m_SceneContext.m_Models.push_back(model);
}

void AScene::AddCamera(const ACamera& camera) {
    m_SceneContext.m_Cameras.push_back(camera);
}