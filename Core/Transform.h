//
// Created by conta on 2/16/2024.
//

#pragma once

#include "Interfaces/Component.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class EAxis : u8 { X, Y, Z };

struct FSceneContext;

class ATransform final : public IComponent {
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
    void SetPosition(const glm::vec3& pos);
    void SetPosition(float x, float y, float z);
    void SetRotation(const glm::vec3& rot);
    void SetRotation(float x, float y, float z);
    void SetScale(const glm::vec3& scale);
    void SetScale(float x, float y, float z);

    void Update(float deltaTime, FSceneContext& sceneContext) override;

    glm::vec3 GetPosition() const {
        return m_Position;
    }

    glm::vec3 GetRotation() const {
        return m_Rotation;
    }

    glm::vec3 GetScale() const {
        return m_Scale;
    }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
    glm::mat4 m_ModelMatrix;
};