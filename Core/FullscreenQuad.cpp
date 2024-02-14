//
// Created by jr on 2/13/24.
//

#include "FullscreenQuad.h"

#include "GraphicsContext.h"
#include "Resources.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace QuadGeometry {
    struct FVertex {
        glm::vec3 Position;
        glm::vec2 TexCoords;
    };

    vector<FVertex> g_Vertices = {{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                                  {{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
                                  {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
                                  {{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}};

    vector<unsigned int> g_Indices = {0, 1, 3, 1, 2, 3};
}  // namespace QuadGeometry

// namespace QuadGeometry

unsigned int g_Texture;

void AFullscreenQuad::Initialize(const string& shaderSrc) {
    m_Shader = new AShader(shaderSrc);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 QuadGeometry::g_Vertices.size() *
                   sizeof(QuadGeometry::FVertex),
                 QuadGeometry::g_Vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 QuadGeometry::g_Indices.size() * sizeof(unsigned int),
                 QuadGeometry::g_Indices.data(),
                 GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(QuadGeometry::FVertex),
                          (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(QuadGeometry::FVertex),
                          (void*)offsetof(QuadGeometry::FVertex, TexCoords));

    glBindVertexArray(0);

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
}

void AFullscreenQuad::Render() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_Texture);

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