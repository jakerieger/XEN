//
// Created by conta on 2/26/2024.
//

#pragma once

#include "Shader.h"
#include "STL.h"
#include <glad/glad.h>

class AQuad {
public:
    AQuad() = default;
    void Init();
    void Draw(u32 texture, float zNear, float zFar) const;
    void Cleanup() const;

private:
    eastl::vector<float> m_Vertices = {
      -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    u32 m_VBO = 0;
    u32 m_VAO = 0;
    eastl::unique_ptr<AShader> m_Shader;
};

inline void AQuad::Init() {
    m_Shader = eastl::unique_ptr<AShader>(new AShader(BuiltinShaders::Quad));

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * m_Vertices.size(),
                 m_Vertices.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          static_cast<void*>(nullptr));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));

    m_Shader->Use();
    m_Shader->SetInt("u_Texture", 0);
}

inline void
AQuad::Draw(const u32 texture, const float zNear, const float zFar) const {
    m_Shader->Use();
    m_Shader->SetFloat("u_NearZ", zNear);
    m_Shader->SetFloat("u_FarZ", zFar);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

inline void AQuad::Cleanup() const {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}
