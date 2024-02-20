//
// Created by conta on 2/18/2024.
//

#include "Monke.h"
#include "InputCodes.h"
#include "Resources.h"
#include "SceneContext.h"

#include "STL.h"

Monke::Monke(const eastl::string& name) : IGameObject(name) {
    auto mat = IMaterial::Create<Materials::BlinnPhong>();
    mat->Cast<Materials::BlinnPhong>()->SetColor({0.f, 0.4f, 0.8f});

    m_MonkeMesh = make_unique<AMeshRenderer>(
      Resources::GetResource(RES_3D_MODEL, "Monke.fbx").c_str(),
      mat);
}

void Monke::Draw(FSceneContext& sceneContext) {
    m_MonkeMesh->Draw(sceneContext, GetTransform());
}

void Monke::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_MonkeMesh->Start(sceneContext);
}

void Monke::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);

    const auto rotationY = 50 * (glfwGetTime() * glm::radians(45.f));
    GetTransform()->SetPositionAndRotation(GetTransform()->GetPosition(),
                                           glm::vec3(0.f, rotationY, 0.f));

    constexpr auto red    = 0.5f;
    const auto green      = 0.5 + 0.5 * sin(glfwGetTime());
    const auto blue       = sin(glfwGetTime());
    const glm::vec3 color = {red, green, blue};
    m_MonkeMesh->GetMaterialAs<Materials::BlinnPhong>()->SetColor(color);

    m_MonkeMesh->Update(deltaTime, sceneContext);
}

void Monke::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_MonkeMesh->Destroyed(sceneContext);
}

void Monke::OnKeyDown(FKeyEvent& event) {
    IInputListener::OnKeyDown(event);

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
