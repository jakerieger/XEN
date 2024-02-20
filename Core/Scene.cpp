//
// Created by jr on 2/16/24.
//

#include "Scene.h"
#include "Interfaces/Drawable.h"

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
    }
}

void AScene::Render() {
    for (const auto& go : m_SceneContext.m_GameObjects) {
        const auto drawable = go->Cast<IDrawable>();
        if (drawable) {
            drawable->Draw(m_SceneContext);
        }
    }
}

ACamera* AScene::GetActiveCamera(FSceneContext& context) {
    for (const auto cameras = FindAllGameObjectsOf<ACamera>(context);
         auto& cam : cameras) {
        if (cam->GetActive()) {
            return cam;
        }
    }

    return nullptr;
}

IGameObject* AScene::FindGameObject(FSceneContext& context,
                                    const string& name) {
    for (const auto& go : context.m_GameObjects) {
        if (go->GetName() == name) {
            return go.get();
        }
    }

    return nullptr;
}