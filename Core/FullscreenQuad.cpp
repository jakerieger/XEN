//
// Created by jr on 2/13/24.
//

#include "FullscreenQuad.h"

#include "GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace QuadGeometry {
    float g_Vertices[] = {
      1.0f,
      1.0f,
      0.0f,  // top right
      1.0f,
      -1.0f,
      0.0f,  // bottom right
      -1.0f,
      -1.0f,
      0.0f,  // bottom left
      -1.0f,
      1.0f,
      1.0f  // top left
    };

    uint32_t g_Indices[] = {0, 1, 3, 1, 2, 3};

    float g_TexCoords[] = {
      0.0f,
      0.0f,  // lower-left corner
      1.0f,
      0.0f,  // lower-right corner
      0.5f,
      1.0f  // top-center corner
    };
}  // namespace QuadGeometry

// namespace QuadGeometry

void AFullscreenQuad::Initialize(const string& shaderSrc) {
    m_Shader = new AShader(shaderSrc);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(QuadGeometry::g_Vertices),
                 QuadGeometry::g_Vertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(QuadGeometry::g_Indices),
                 QuadGeometry::g_Indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    m_Shader->Use();
    m_Shader->SetVec2("u_Resolution",
                      Graphics::GetWindowSize().Width,
                      Graphics::GetWindowSize().Height);
}

void AFullscreenQuad::Render() const {
    m_Shader->Use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void AFullscreenQuad::Update(float deltaTime) {
    m_Shader->Use();
    m_Shader->SetFloat("u_Time", glfwGetTime());
    m_Shader->SetFloat("u_DeltaTime", deltaTime);
}

void AFullscreenQuad::Destroy() const {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    m_Shader->Destroy();
    delete m_Shader;
}