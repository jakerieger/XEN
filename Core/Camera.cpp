//
// Created by jr on 2/15/24.
//

#include "Camera.h"
#include "Engine/GraphicsContext.h"
#include "InputCodes.h"
#include "SceneContext.h"

ACamera::ACamera(const eastl::string& name,
                 const glm::vec3& up,
                 const float yaw,
                 const float pitch,
                 const float fov)
    : IGameObject(name), m_Front({0.f, 0.f, -1.f}), m_FOV(fov) {
    m_WorldUp = up;
    m_Yaw     = yaw;
    m_Pitch   = pitch;
    UpdateCameraVectors();
    m_LastX               = Graphics::GetWindowSize().Width / 2;
    m_LastY               = Graphics::GetWindowSize().Height / 2;
    m_ShadowCascadeLevels = {m_FarClip / 50.f,
                             m_FarClip / 25.f,
                             m_FarClip / 10.f,
                             m_FarClip / 2.f};
}

ACamera::ACamera(const eastl::string& name,
                 float upX,
                 float upY,
                 float upZ,
                 const float yaw,
                 const float pitch,
                 const float fov)
    : IGameObject(name), m_Front({0.f, 0.f, -1.f}), m_FOV(fov) {
    m_WorldUp = {upX, upY, upZ};
    m_Yaw     = yaw;
    m_Pitch   = pitch;
    UpdateCameraVectors();
    m_LastX               = Graphics::GetWindowSize().Width / 2;
    m_LastY               = Graphics::GetWindowSize().Height / 2;
    m_ShadowCascadeLevels = {m_FarClip / 50.f,
                             m_FarClip / 25.f,
                             m_FarClip / 10.f,
                             m_FarClip / 2.f};
}

glm::mat4 ACamera::GetViewMatrix() {
    return lookAt(GetTransform()->GetPosition(),
                  GetTransform()->GetPosition() + m_Front,
                  m_Up);
}

glm::mat4 ACamera::GetProjectionMatrix(const float aspect) const {
    return glm::perspective(glm::radians(m_FOV), aspect, m_NearClip, m_FarClip);
}

glm::mat4 ACamera::GetProjectionMatrix(const float aspect,
                                       const float zNear,
                                       const float zFar) const {
    return glm::perspective(glm::radians(m_FOV), aspect, zNear, zFar);
}

void ACamera::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    UpdateCameraVectors();
}

void ACamera::OnScroll(FScrollEvent& event) {
    IGameObject::OnScroll(event);

    if (event.Y > 0) {
        m_FOV += 4;
    } else {
        m_FOV -= 4;
    }
}

void ACamera::OnMouseMove(FMouseMoveEvent& event) {
    IGameObject::OnMouseMove(event);

    const float xPos = event.X;
    const float yPos = event.Y;

    if (m_FirstMouse) {
        m_LastX      = xPos;
        m_LastY      = yPos;
        m_FirstMouse = false;
    }

    const float xOffset = (xPos - m_LastX) * m_MouseSensitivity;
    const float yOffset = (m_LastY - yPos) * m_MouseSensitivity;
    m_LastX             = xPos;
    m_LastY             = yPos;

    m_Yaw += xOffset;
    m_Pitch += yOffset;

    // Contrain pitch
    if (m_Pitch > 89.f) {
        m_Pitch = 89.f;
    }
    if (m_Pitch < -89.f) {
        m_Pitch = -89.f;
    }
}

void ACamera::OnKey(FKeyEvent& event) {
    IGameObject::OnKey(event);

    const float velocity = 100.f * Graphics::GetDeltaTime();
    if (event.KeyCode == KeyCode::W) {
        GetTransform()->Translate(m_Front * velocity);
    }
    if (event.KeyCode == KeyCode::A) {
        // GetTransform()->Translate((m_Front * -velocity));
    }
    if (event.KeyCode == KeyCode::S) {
        // GetTransform()->Translate(m_Front * velocity);
    }
    if (event.KeyCode == KeyCode::D) {
        // GetTransform()->Translate(m_Front * velocity);
    }
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