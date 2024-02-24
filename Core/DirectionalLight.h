//
// Created by conta on 2/16/2024.
//

#pragma once
#include "Transform.h"

#include <EASTL/unique_ptr.h>

class ADirectionalLight {
public:
    explicit ADirectionalLight(const glm::vec3& direction,
                               const glm::vec3& color = {1.f, 1.f, 1.f},
                               const float strength   = 1.f)
        : m_Direction(direction), m_Color(color), m_Strength(strength) {}

    static eastl::unique_ptr<ADirectionalLight>
    Create(const glm::vec3& direction) {
        return eastl::make_unique<ADirectionalLight>(direction);
    }

    glm::vec3 GetDirection() const {
        return normalize(m_Direction);
    }

    glm::vec3 GetColor() {
        return m_Color;
    }

    float GetStrength() const {
        return m_Strength;
    }

private:
    glm::vec3 m_Direction;
    glm::vec3 m_Color;
    float m_Strength;
};