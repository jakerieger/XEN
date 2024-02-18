//
// Created by conta on 2/18/2024.
//

#include "Monke.h"
#include "Resources.h"
#include "SceneContext.h"

#include <GLFW/glfw3.h>

Monke::Monke(const uint32_t id) : IGameObject(id) {
    m_MeshRenderer = new AMeshRenderer(
      Resources::GetResource(RES_3D_MODEL, "monk.fbx").c_str(),
      new Materials::BlinnPhong(),
      GetTransform());

    this->SetMeshRenderer(m_MeshRenderer);
}

void Monke::Draw(FSceneContext& sceneContext) {
    m_MeshRenderer->Draw(sceneContext);
}

void Monke::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_MeshRenderer->Start(sceneContext);
}

void Monke::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);

    const auto rotationY = 50 * (glfwGetTime() * glm::radians(45.f));
    GetTransform()->SetPositionAndRotation(glm::vec3(0.f),
                                           glm::vec3(0.f, rotationY, 0.f));

    m_MeshRenderer->Update(deltaTime, sceneContext);
}

void Monke::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_MeshRenderer->Destroyed(sceneContext);

    delete m_MeshRenderer;
}