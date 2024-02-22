//
// Created by conta on 2/18/2024.
//

#pragma once

#include "Interfaces/Drawable.h"
#include "Interfaces/GameObject.h"
#include "MeshRenderer.h"

class Statue final : public IGameObject,
                     public IDrawable {
public:
    explicit Statue(const eastl::string& name);

    // IDrawable
    void Draw(FSceneContext& sceneContext) override;

    // IGameObject
    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    eastl::unique_ptr<AMeshRenderer> m_Mesh;
};