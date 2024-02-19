//
// Created by conta on 2/18/2024.
//

#pragma once

#include "Interfaces/Drawable.h"
#include "Interfaces/GameObject.h"
#include "MeshRenderer.h"

class Monke final : public IGameObject,
                    public IDrawable {
public:
    explicit Monke(const string& name);

    // IDrawable
    void Draw(FSceneContext& sceneContext) override;

    // IGameObject
    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    // IInputListener
    void OnKeyDown(FKeyEvent& event) override;

    AMeshRenderer* m_MeshRenderer = nullptr;
};