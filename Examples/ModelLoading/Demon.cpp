//
// Created by conta on 2/18/2024.
//

#include "Demon.h"
#include "Resources.h"
#include "SceneContext.h"

#include "STL.h"

Demon::Demon(const eastl::string& name) : IGameObject(name) {
    auto mat = eastl::unique_ptr<IMaterial>(
      new Materials::BlinnPhong("Resources/Textures/MetalBronzeWorn001/"
                                "MetalBronzeWorn001_COL_2K_METALNESS.png",
                                "Resources/Textures/MetalBronzeWorn001/"
                                "MetalBronzeWorn001_NRM_2K_METALNESS.png"));

    m_Demon = make_unique<AMeshRenderer>(
      Resources::GetResource(RES_3D_MODEL, "Snowden.fbx").c_str(),
      mat);
    GetTransform()->SetScale(0.3, 0.3, 0.3);
}

void Demon::Draw(FSceneContext& sceneContext) {
    m_Demon->Draw(sceneContext, GetTransform());
}

void Demon::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_Demon->Start(sceneContext);
}

void Demon::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);

    const auto rotationY = 50 * (glfwGetTime() * glm::radians(45.f));
    GetTransform()->SetPositionAndRotation(GetTransform()->GetPosition(),
                                           glm::vec3(0.f, rotationY, 0.f));

    m_Demon->Update(deltaTime, sceneContext);
}

void Demon::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_Demon->Destroyed(sceneContext);
}