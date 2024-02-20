//
// Created by conta on 2/19/2024.
//

#include "Floor.h"

#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"

Floor::Floor(const string& name) : IGameObject(name) {
    auto mat = IMaterial::Create<Materials::BlinnPhong>();

    m_FloorMesh = make_unique<AMeshRenderer>(
      Resources::GetResource(RES_3D_MODEL, "Floor.fbx").c_str(),
      mat);
}

void Floor::Draw(FSceneContext& sceneContext) {
    m_FloorMesh->Draw(sceneContext, GetTransform());
}

void Floor::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_FloorMesh->Start(sceneContext);
}

void Floor::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    m_FloorMesh->Update(deltaTime, sceneContext);
}

void Floor::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_FloorMesh->Destroyed(sceneContext);
}