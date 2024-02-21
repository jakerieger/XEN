//
// Created by conta on 2/18/2024.
//

#include "TV.h"
#include "InputCodes.h"
#include "Resources.h"
#include "SceneContext.h"

#include "STL.h"

TV::TV(const eastl::string& name) : IGameObject(name) {
    auto mat = IMaterial::Create<Materials::BlinnPhong>();

    m_TV = make_unique<AMeshRenderer>(
      Resources::GetResource(RES_3D_MODEL, "TV.fbx").c_str(),
      mat);
}

void TV::Draw(FSceneContext& sceneContext) {
    m_TV->Draw(sceneContext, GetTransform());
}

void TV::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_TV->Start(sceneContext);
}

void TV::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);

    const auto rotationY = 50 * (glfwGetTime() * glm::radians(45.f));
    GetTransform()->SetPositionAndRotation(GetTransform()->GetPosition(),
                                           glm::vec3(0.f, rotationY, 0.f));

    m_TV->Update(deltaTime, sceneContext);
}

void TV::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_TV->Destroyed(sceneContext);
}