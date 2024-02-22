//
// Created by conta on 2/21/2024.
//

#include "Skybox.h"
#include "Resources.h"
#include "Shader.h"
#include "Camera.h"
#include "GraphicsContext.h"

#include <glad/glad.h>
#include <stb_image.h>

const eastl::vector g_SkyboxVertices = {
  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,
  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,
  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,
  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

ASkybox::ASkybox() : m_VAO(0), m_VBO(0) {
    auto right  = "Resources/Textures/BlueSunset/right.jpg";
    auto left   = "Resources/Textures/BlueSunset/left.jpg";
    auto top    = "Resources/Textures/BlueSunset/top.jpg";
    auto bottom = "Resources/Textures/BlueSunset/bottom.jpg";
    auto front  = "Resources/Textures/BlueSunset/front.jpg";
    auto back   = "Resources/Textures/BlueSunset/back.jpg";

    m_CubemapTexture = LoadCubemap({right, left, top, bottom, front, back});
    m_Shader         = eastl::make_unique<AShader>(BuiltinShaders::Skybox);
}

void ASkybox::Init() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * g_SkyboxVertices.size(),
                 g_SkyboxVertices.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          static_cast<void*>(nullptr));

    m_Shader->Use();
    m_Shader->SetInt("u_Skybox", 0);
}

void ASkybox::Draw(ACamera* camera) const {
    glDepthFunc(GL_LEQUAL);
    m_Shader->Use();
    m_Shader->SetMat4("u_Projection",
                      camera->GetProjectionMatrix(Graphics::GetWindowAspect()));
    m_Shader->SetMat4("u_View", glm::mat4(glm::mat3(camera->GetViewMatrix())));

    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void ASkybox::Destroy() const {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

u32 ASkybox::LoadCubemap(const eastl::vector<eastl::string>& faces) {
    u32 textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    i32 width, height, nrChannels;
    for (u32 i = 0; i < faces.size(); i++) {
        auto face = faces[i];
        u8* data  = stbi_load(face.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         GL_RGB,
                         width,
                         height,
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         data);
            stbi_image_free(data);
        } else {
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}