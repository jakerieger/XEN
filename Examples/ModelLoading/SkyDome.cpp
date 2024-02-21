//
// Created by conta on 2/20/2024.
//

#include "SkyDome.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"

SkyDome::SkyDome(const eastl::string& name) : IGameObject(name) {
    auto mat = IMaterial::Create<Materials::SkyDome>();

    m_SkyDomeMesh = make_unique<AMeshRenderer>(
      Resources::GetResource(RES_3D_MODEL, "SkyDome.fbx").c_str(),
      mat);
}

void SkyDome::Draw(FSceneContext& sceneContext) {
    m_SkyDomeMesh->Draw(sceneContext, GetTransform());
}

void SkyDome::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_SkyDomeMesh->Start(sceneContext);
}

void SkyDome::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    m_SkyDomeMesh->Update(deltaTime, sceneContext);
}

void SkyDome::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_SkyDomeMesh->Destroyed(sceneContext);
}