//
// Created by jr on 2/12/24.
//
#include <catch2/catch_test_macros.hpp>
#include "GameObject.h"
#include "Component.h"
#include "SceneContext.h"
#include "STL.h"

class TestGameObject final : public IGameObject {
public:
    TestGameObject() : IGameObject(0) {}

    void Awake(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
};

void TestGameObject::Awake(FSceneContext& sceneContext) {
    IGameObject::Awake(sceneContext);


}

void TestGameObject::Update(const float deltaTime,
                            FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);


}

class TestComponent final : public IComponent {
public:
    void Awake(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
};

void TestComponent::Awake(FSceneContext& sceneContext) {

}

void TestComponent::Update(float deltaTime, FSceneContext& sceneContext) {

}

TEST_CASE("Lifecycle Hooks", "[GameObject]") {
    FSceneContext context;
    context.m_Sun = ADirectionalLight();

    TestGameObject gameObj;
    TestComponent comp;

    gameObj.AddComponent(comp);

    gameObj.Awake(context);
    gameObj.Update(0.f, context);
}