//
// Created by jr on 2/15/24.
//

#include "Camera.h"

ACamera::ACamera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
    : m_Front({0.f, 0.f, -1.f}) {
    m_Position = pos;
    m_WorldUp  = up;
    m_Yaw      = yaw;
    m_Pitch    = pitch;
    UpdateCameraVectors();
}

ACamera::ACamera(float posX,
                 float posY,
                 float posZ,
                 float upX,
                 float upY,
                 float upZ,
                 float yaw,
                 float pitch)
    : m_Front({0.f, 0.f, -1.f}) {
    m_Position = {posX, posY, posZ};
    m_WorldUp  = {upX, upY, upZ};
    m_Yaw      = yaw;
    m_Pitch    = pitch;
    UpdateCameraVectors();
}

glm::mat4 ACamera::GetViewMatrix() const {
    return lookAt(m_Position, m_Front, m_Up);
}

glm::mat4 ACamera::GetProjectionMatrix(const float fov, float aspect) {
    return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
}

void ACamera::UpdateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = normalize(front);
    // also re-calculate the Right and Up vector
    m_Right = normalize(
      cross(m_Front,
            m_WorldUp));  // normalize the vectors, because their length
                          // gets closer to 0 the more you look up or down
                          // which results in slower movement.
    m_Up = normalize(cross(m_Right, m_Front));
}