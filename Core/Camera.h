//
// Created by jr on 2/15/24.
//

#pragma once

#include "GameObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct FSceneContext;

class ACamera final : public IGameObject {
public:
    explicit ACamera(const string& name,
                     glm::vec3 up = {0.f, 1.f, 0.f},
                     float yaw    = -90.f,
                     float pitch  = 0.f,
                     float fov    = 45.f);
    ACamera(const string& name,
            float upX,
            float upY,
            float upZ,
            float yaw,
            float pitch,
            float fov = 45.f);

    void SetActive(const bool active) {
        m_IsActiveCamera = active;
    }

    bool GetActive() const {
        return m_IsActiveCamera;
    }

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float aspect) const;

    void Update(const float deltaTime, FSceneContext& sceneContext) override;
    void OnScroll(FScrollEvent& event) override;

private:
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    float m_Yaw;
    float m_Pitch;
    bool m_IsActiveCamera = false;
    float m_FOV;
    // float m_AspectRatio;

    void UpdateCameraVectors();
};