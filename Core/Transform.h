//
// Created by conta on 2/16/2024.
//

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class EAxis : uint8_t { X, Y, Z };

class ATransform {
public:
    ATransform();

    glm::mat4 GetModelMatrix() const {
        return m_ModelMatrix;
    }

    glm::vec3 GetEulerAngles();

    void Translate(const glm::vec3& translation);
    void Translate(float x, float y, float z);
    void Rotate(float angle, EAxis axis);
    void Scale(const glm::vec3& scale);
    void Scale(float x, float y, float z);
    void SetPositionAndRotation(const glm::vec3& position,
                                const glm::vec3& rotation);
    void SetPositionAndRotation(
      float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
    void Update();

    glm::vec3 GetPosition() {
        return m_Position;
    }
    glm::vec3 GetRotation() {
        return m_Rotation;
    }
    glm::vec3 GetScale() {
        return m_Scale;
    }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
    glm::mat4 m_ModelMatrix;
};