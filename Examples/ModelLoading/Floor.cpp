//
// Created by conta on 2/19/2024.
//

#include "Floor.h"

#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"

Floor::Floor(const eastl::string& name) : IGameObject(name) {
    auto mat = eastl::unique_ptr<IMaterial>(new Materials::BlinnPhong(
      "Resources/Textures/ConcretePrecastPlates004/"
      "ConcretePrecastPlates004_COL_2K_METALNESS.png",
      "Resources/Textures/ConcretePrecastPlates004/"
      "ConcretePrecastPlates004_NRM_2K_METALNESS.png"));

    m_FloorMesh = make_unique<AMeshRenderer>(
      Resources::GetResource(RES_3D_MODEL, "Floor.assbin").c_str(),
      mat);
    m_FloorMesh->SetParent(this);

    RegisterComponent(m_FloorMesh.get());
}

void Floor::Draw(FSceneContext& sceneContext) {
    m_FloorMesh->Draw(sceneContext, GetTransform());
}

void Floor::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_FloorMesh->Start(sceneContext);
    m_FloorMesh->GetMaterialAs<Materials::BlinnPhong>()->SetUVScale(600.f);
    m_FloorMesh->GetMaterialAs<Materials::BlinnPhong>()->SetColor(
      glm::vec3(1.f));
}

void Floor::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    m_FloorMesh->Update(deltaTime, sceneContext);
}

void Floor::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_FloorMesh->Destroyed(sceneContext);
}