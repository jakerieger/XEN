//
// Created by jr on 2/15/24.
//

#pragma once

#include "Interfaces/GameObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct FSceneContext;

class ACamera final : public IGameObject {
public:
    explicit ACamera(const eastl::string& name,
                     const glm::vec3& up = {0.f, 1.f, 0.f},
                     float yaw           = -90.f,
                     float pitch         = 0.f,
                     float fov           = 60.f);
    ACamera(const eastl::string& name,
            float upX,
            float upY,
            float upZ,
            float yaw,
            float pitch,
            float fov = 60.f);

    void SetActive(const bool active) {
        m_IsActiveCamera = active;
    }

    bool GetActive() const {
        return m_IsActiveCamera;
    }

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float aspect) const;
    glm::mat4 GetProjectionMatrix(float aspect, float zNear, float zFar) const;

    /**
     * \brief Get camera near and far clipping planes
     * \return Clipping planes as <Near, Far>
     */
    eastl::tuple<float, float> GetClippingPlanes() const {
        return eastl::make_tuple(m_NearClip, m_FarClip);
    }

    void Update(const float deltaTime, FSceneContext& sceneContext) override;
    void OnScroll(FScrollEvent& event) override;
    void OnMouseMove(FMouseMoveEvent& event) override;
    void OnKey(FKeyEvent& event) override;

private:
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    float m_NearClip = 0.1f;
    float m_FarClip  = 1000.f;
    float m_Yaw;
    float m_Pitch;
    bool m_IsActiveCamera = false;
    float m_FOV;
    bool m_FirstMouse        = true;
    float m_LastX            = 0.f;
    float m_LastY            = 0.f;
    float m_MouseSensitivity = 0.4f;
    eastl::vector<float> m_ShadowCascadeLevels;
    // float m_AspectRatio;

    void UpdateCameraVectors();
};