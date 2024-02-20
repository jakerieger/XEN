//
// Created by jr on 2/16/24.
//

#include "Material.h"

#include "Resources.h"

#include <glad/glad.h>
#include "stb_image.h"

namespace Materials {
    BlinnPhong::BlinnPhong()
        : IMaterial(make_unique<AShader>(BuiltinShaders::BlinnPhong)) {}

    void BlinnPhong::Initialize() {
        glGenTextures(1, &m_DiffuseTexture);
        glBindTexture(GL_TEXTURE_2D, m_DiffuseTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        u8* data =
          stbi_load(Resources::GetResource(RES_TEXTURE, "checker.jpg").c_str(),
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
    }

    void BlinnPhong::Use() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_DiffuseTexture);
        m_Shader->Use();
    }

    void BlinnPhong::UpdateUniforms() {
        m_Shader->SetVec3("u_ObjectColor", m_DiffuseColor);
        m_Shader->SetInt("u_DiffuseMap", 0);
    }

    void BlinnPhong::Destroy() {}

    Unlit::Unlit() : IMaterial(make_unique<AShader>(BuiltinShaders::Unlit)) {}

    void Unlit::Initialize() {}

    void Unlit::Use() {
        m_Shader->Use();
    }

    void Unlit::UpdateUniforms() {
        m_Shader->SetVec3("u_ObjectColor", m_Color);
    }

    void Unlit::Destroy() {}

}  // namespace Materials