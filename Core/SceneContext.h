//
// Created by jr on 2/16/24.
//

#pragma once

#include "Camera.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "STL.h"

struct FSceneContext {
    vector<AModel> m_Models;
    vector<ACamera> m_Cameras;
    ADirectionalLight m_Sun;
};