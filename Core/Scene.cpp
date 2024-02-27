//
// Created by jr on 2/16/24.
//

#include "Scene.h"

#include "Engine/GraphicsContext.h"
#include "Interfaces/Drawable.h"
#include <glad/glad.h>

static void InitFramebuffer(u32& fbo);
static void InitDepthMap(u32& texture);
static void BindDepthBuffer(const u32& fbo, const u32& map);

AScene::AScene(const eastl::string& name) : m_Name(name) {
    InitFramebuffer(m_SceneContext.DepthFBO);
    InitDepthMap(m_SceneContext.DepthMap);
    BindDepthBuffer(m_SceneContext.DepthFBO, m_SceneContext.DepthMap);
}

void AScene::Awake() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->Awake(m_SceneContext);
    }
}

void AScene::Start() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->Start(m_SceneContext);
    }
}

void AScene::Update(const float deltaTime) {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->Update(deltaTime, m_SceneContext);
    }
}

void AScene::LateUpdate() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->LateUpdate(m_SceneContext);
    }
}

void AScene::FixedUpdated() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->FixedUpdated(m_SceneContext);
    }
}

void AScene::Destroyed() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->Destroyed(m_SceneContext);
    }
}

void AScene::DrawDepthPass() {
    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
    glBindFramebuffer(GL_FRAMEBUFFER, m_SceneContext.DepthFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    for (const auto& go : m_SceneContext.GameObjects) {
        const auto drawable = go->Cast<IDrawable>();
        if (drawable) {
            drawable->DrawDepth(m_SceneContext);
        }
    }
}

void AScene::DrawMainPass() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,
               0,
               Graphics::GetWindowSize().Width,
               Graphics::GetWindowSize().Height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, m_SceneContext.DepthMap);
    for (const auto& go : m_SceneContext.GameObjects) {
        const auto drawable = go->Cast<IDrawable>();
        if (drawable) {
            drawable->Draw(m_SceneContext);
        }
    }
}

void AScene::Render() {
    DrawDepthPass();
    DrawMainPass();
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
                                    const eastl::string& name) {
    for (const auto& go : context.GameObjects) {
        if (go->GetName() == name) {
            return go.get();
        }
    }

    return nullptr;
}

// HELPERS //
void InitFramebuffer(u32& fbo) {
    glGenFramebuffers(1, &fbo);
}

void InitDepthMap(u32& texture) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT,
                 SHADOW_SIZE,
                 SHADOW_SIZE,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void BindDepthBuffer(const u32& fbo, const u32& map) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           map,
                           0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}