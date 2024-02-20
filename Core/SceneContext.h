//
// Created by jr on 2/16/24.
//

#pragma once

#include "DirectionalLight.h"
#include "Interfaces/GameObject.h"
#include "STL.h"

struct FSceneContext {
    eastl::vector<eastl::unique_ptr<IGameObject>> m_GameObjects;
    ADirectionalLight m_Sun;
};