//
// Created by conta on 2/20/2024.
//

#pragma once

#include "Interfaces/GameObject.h"
#include "Interfaces/Drawable.h"

class SkyDome final : public IGameObject,
                      public IDrawable {
public:
    explicit SkyDome(const eastl::string& name);

    void Draw(FSceneContext& sceneContext) override;
    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    eastl::unique_ptr<AMeshRenderer> m_SkyDomeMesh;
};