//
// Created by conta on 2/16/2024.
//

#include "Transform.h"

ATransform::ATransform() {
    m_ModelMatrix = glm::mat4(1.f);
    m_Position    = glm::vec3(0.f);
    m_Rotation    = glm::vec3(0.f);
    m_Scale       = glm::vec3(1.f);
}

glm::vec3 ATransform::GetEulerAngles() {
    glm::vec3 eulerAngles;
    eulerAngles.x =
      glm::degrees(glm::atan(m_ModelMatrix[1][0] / m_ModelMatrix[0][0]));
    eulerAngles.y = glm::degrees(glm::asin(-m_ModelMatrix[2][0]));
    eulerAngles.z =
      glm::degrees(glm::atan(m_ModelMatrix[2][1] / m_ModelMatrix[2][2]));
    return eulerAngles;
}

void ATransform::Translate(const glm::vec3& translation) {
    m_Position += translation;
}

void ATransform::Translate(const float x, const float y, const float z) {
    const glm::vec3 trans = {x, y, z};
    Translate(trans);
}

void ATransform::Rotate(const float angle, const EAxis axis) {
    switch (axis) {
        case EAxis::X:
            m_Rotation.x += angle;
            break;
        case EAxis::Y:
            m_Rotation.y += angle;
            break;
        case EAxis::Z:
            m_Rotation.z += angle;
            break;
    }
}

void ATransform::Scale(const glm::vec3& scale) {
    m_Scale *= scale;
}

void ATransform::Scale(const float x, const float y, const float z) {
    const glm::vec3 scale = {x, y, z};
    Scale(scale);
}

void ATransform::SetPositionAndRotation(const glm::vec3& position,
                                        const glm::vec3& rotation) {
    m_Position = position;
    m_Rotation = rotation;
}

void ATransform::SetPositionAndRotation(const float posX,
                                        const float posY,
                                        const float posZ,
                                        const float rotX,
                                        const float rotY,
                                        const float rotZ) {
    const glm::vec3 pos = {posX, posY, posZ};
    const glm::vec3 rot = {rotX, rotY, rotZ};
    SetPositionAndRotation(pos, rot);
}

void ATransform::Update(float deltaTime, FSceneContext& sceneContext) {
    m_ModelMatrix = glm::mat4(1.0f);
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::rotate(m_ModelMatrix,
                                glm::radians(m_Rotation.x),
                                glm::vec3(1.0f, 0.0f, 0.0f));
    m_ModelMatrix = glm::rotate(m_ModelMatrix,
                                glm::radians(m_Rotation.y),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    m_ModelMatrix = glm::rotate(m_ModelMatrix,
                                glm::radians(m_Rotation.z),
                                glm::vec3(0.0f, 0.0f, 1.0f));
    m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
}