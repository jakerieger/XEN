//
// Created by jr on 2/16/24.
//

#pragma once

#include "DirectionalLight.h"
#include "Interfaces/GameObject.h"
#include "STL.h"

struct FSceneContext {
    eastl::vector<eastl::unique_ptr<IGameObject>> GameObjects;
    eastl::unique_ptr<ADirectionalLight> Sun;
    u32 DepthFBO = 0;
    u32 DepthMap = 0;
};