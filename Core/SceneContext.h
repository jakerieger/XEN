//
// Created by jr on 2/16/24.
//

#pragma once

#include "DirectionalLight.h"
#include "GameObject.h"
#include "STL.h"

struct FSceneContext {
    vector<IGameObject*> m_GameObjects;
    ADirectionalLight m_Sun;
};