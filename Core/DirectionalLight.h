//
// Created by conta on 2/16/2024.
//

#pragma once
#include "Transform.h"

class ADirectionalLight {
public:
    explicit ADirectionalLight(const glm::vec3& color = {1.f, 1.f, 1.f},
                               const float strength   = 100.f)
        : m_Color(color), m_Strength(strength) {}

    ATransform& GetTransform() {
        return m_Transform;
    }

    glm::vec3 GetColor() {
        return m_Color;
    }

private:
    ATransform m_Transform;
    glm::vec3 m_Color;
    float m_Strength;
};