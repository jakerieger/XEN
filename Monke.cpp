//
// Created by conta on 2/18/2024.
//

#include "Monke.h"

#include "Input.h"
#include "KeyCode.h"
#include "Resources.h"
#include "SceneContext.h"

#include <GLFW/glfw3.h>

Monke::Monke(const string& name) : IGameObject(name) {
    m_MeshRenderer = new AMeshRenderer(
      Resources::GetResource(RES_3D_MODEL, "monk.fbx").c_str(),
      new Materials::BlinnPhong(),
      GetTransform());

    this->SetMeshRenderer(m_MeshRenderer);
    Input::RegisterListener(this);
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
    GetTransform()->SetPositionAndRotation(GetTransform()->GetPosition(),
                                           glm::vec3(0.f, rotationY, 0.f));

    m_MeshRenderer->Update(deltaTime, sceneContext);
}

void Monke::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_MeshRenderer->Destroyed(sceneContext);

    delete m_MeshRenderer;
}

void Monke::OnKeyDown(FKeyEvent& event) {
    IInputListener::OnKeyDown(event);

    if (event.Modifiers == 0 && event.Action == GLFW_PRESS)
        switch (event.KeyCode) {
            case KeyCode::W:
                GetTransform()->Translate(0.f, 0.f, 0.25f);
                break;
            case KeyCode::S:
                GetTransform()->Translate(0.f, 0.f, -0.25f);
                break;
            case KeyCode::A:
                GetTransform()->Translate(0.25f, 0.f, 0.f);
                break;
            case KeyCode::D:
                GetTransform()->Translate(-0.25f, 0.f, 0.f);
                break;
            default:
                break;
        }
}
