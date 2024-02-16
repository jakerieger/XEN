//
// Created by jr on 2/15/24.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "STL.h"

class ACamera {
public:
    explicit ACamera(glm::vec3 pos = {0.f, 0.f, 0.f},
                     glm::vec3 up  = {0.f, 1.f, 0.f},
                     float yaw     = -90.f,
                     float pitch   = 0.f);
    ACamera(float posX,
            float posY,
            float posZ,
            float upX,
            float upY,
            float upZ,
            float yaw,
            float pitch);

    glm::mat4 GetViewMatrix() const;
    static glm::mat4 GetProjectionMatrix(float fov, float aspect);

private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    float m_Yaw;
    float m_Pitch;
    // float m_FOV;
    // float m_AspectRatio;

    void UpdateCameraVectors();
};