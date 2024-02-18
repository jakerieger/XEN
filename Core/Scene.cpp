//
// Created by jr on 2/16/24.
//

#include "Scene.h"
#include "Drawable.h"

void AScene::Awake() {
    for (const auto& go : m_SceneContext.m_GameObjects) {
        go->Awake(m_SceneContext);
    }
}

void AScene::Start() {

    for (const auto& go : m_SceneContext.m_GameObjects) {
        go->Start(m_SceneContext);
    }
}

void AScene::Update(const float deltaTime) {

    for (const auto& go : m_SceneContext.m_GameObjects) {
        go->Update(deltaTime, m_SceneContext);
    }
}

void AScene::LateUpdate() {

    for (const auto& go : m_SceneContext.m_GameObjects) {
        go->LateUpdate(m_SceneContext);
    }
}

void AScene::FixedUpdated() {

    for (const auto& go : m_SceneContext.m_GameObjects) {
        go->FixedUpdated(m_SceneContext);
    }
}

void AScene::Destroyed() {

    for (const auto& go : m_SceneContext.m_GameObjects) {
        go->Destroyed(m_SceneContext);
        delete go;
    }
}

void AScene::Render() {
    for (const auto& go : m_SceneContext.m_GameObjects) {
        const auto drawable = dynamic_cast<IDrawable*>(go);
        if (drawable) {
            drawable->Draw(m_SceneContext);
        }
    }
}

ACamera* AScene::GetActiveCamera(FSceneContext& context) {
    for (const auto go : context.m_GameObjects) {
        if (const auto camera = dynamic_cast<ACamera*>(go);
            camera && camera->GetActive()) {
            return camera;
        }
    }

    return nullptr;
}