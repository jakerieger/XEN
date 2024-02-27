//
// Created by conta on 2/18/2024.
//

#include "Statue.h"
#include "Engine/Resources.h"
#include "SceneContext.h"

#include "STL.h"

Statue::Statue(const eastl::string& name) : IGameObject(name) {
    auto mat = eastl::unique_ptr<IMaterial>(
      new Materials::BlinnPhong("Resources/Textures/MetalBronzeWorn001/"
                                "MetalBronzeWorn001_COL_2K_METALNESS.png",
                                "Resources/Textures/MetalBronzeWorn001/"
                                "MetalBronzeWorn001_NRM_2K_METALNESS.png"));

    m_Mesh = make_unique<AMeshRenderer>("Resources/Models/Snowden.assbin", mat);
    GetTransform()->SetScale(0.3, 0.3, 0.3);
    m_Mesh->SetParent(this);

    RegisterComponent(m_Mesh.get());
}

void Statue::DrawDepth(FSceneContext& sceneContext) {
    m_Mesh->Draw(sceneContext, GetTransform(), EDrawPass::PASS_DEPTH);
}

void Statue::Draw(FSceneContext& sceneContext) {
    m_Mesh->Draw(sceneContext, GetTransform(), EDrawPass::PASS_MAIN);
}

void Statue::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_Mesh->Start(sceneContext);
}

void Statue::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);

    const auto rotationY = 50 * (glfwGetTime() * glm::radians(45.f));
    GetTransform()->SetPositionAndRotation(GetTransform()->GetPosition(),
                                           glm::vec3(0.f, rotationY, 0.f));

    m_Mesh->Update(deltaTime, sceneContext);
}

void Statue::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_Mesh->Destroyed(sceneContext);
}