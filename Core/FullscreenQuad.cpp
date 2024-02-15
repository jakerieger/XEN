//
// Created by jr on 2/13/24.
//

#include "FullscreenQuad.h"
#include "GraphicsContext.h"
#include "Resources.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

unsigned int g_Texture;

void AFullscreenQuad::Initialize(const string& shaderSrc) {
    m_Mesh   = new AMesh({{{1.0f, 1.0f, 0.0f}, {0.f, 0.f, 0.f}, {1.0f, 1.0f}},
                          {{1.0f, -1.0f, 0.0f}, {0.f, 0.f, 0.f}, {1.0f, 0.0f}},
                          {{-1.0f, -1.0f, 0.0f}, {0.f, 0.f, 0.f}, {0.0f, 0.0f}},
                          {{-1.0f, 1.0f, 0.0f}, {0.f, 0.f, 0.f}, {0.0f, 1.0f}}},
                         {0, 1, 3, 1, 2, 3});
    m_Shader = new AShader(shaderSrc);

    // Import a test texture
    glGenTextures(1, &g_Texture);
    glBindTexture(GL_TEXTURE_2D, g_Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data =
      stbi_load(Resources::GetResource(RES_TEXTURE, "checker_map.png").c_str(),
                &width,
                &height,
                &nrChannels,
                0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     width,
                     height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    m_Shader->Use();
    m_Shader->SetVec2("u_Resolution",
                      Graphics::GetWindowSize().Width,
                      Graphics::GetWindowSize().Height);
    m_Shader->SetInt("u_Texture_0", 0);

    glm::mat4 trans = glm::mat4(1.0f);
    trans           = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    m_Shader->SetMat4("u_Transform", trans);
}

void AFullscreenQuad::Render() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_Texture);

    m_Mesh->Draw(*m_Shader);
}

void AFullscreenQuad::Update(const float deltaTime) const {
    m_Shader->Use();
    m_Shader->SetFloat("u_Time", glfwGetTime());
    m_Shader->SetFloat("u_DeltaTime", deltaTime);
}

void AFullscreenQuad::Destroy() const {
    m_Mesh->Destroy();
    delete m_Mesh;
    m_Shader->Destroy();
    delete m_Shader;
}