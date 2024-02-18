//
// Created by conta on 2/18/2024.
//

#pragma once

#include "Drawable.h"
#include "GameObject.h"
#include "InputListener.h"
#include "MeshRenderer.h"

class Monke final : public IGameObject,
                    public IDrawable,
                    public IInputListener {
public:
    explicit Monke(const uint32_t id);

    void Draw(FSceneContext& sceneContext) override;
    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    AMeshRenderer* m_MeshRenderer = nullptr;
};