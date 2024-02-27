//
// Created by jr on 2/16/24.
//

#include "Material.h"
#include "Engine/Resources.h"
#include "SceneContext.h"
#include "Camera.h"
#include <glad/glad.h>
#include "stb_image.h"

namespace Materials {
    BlinnPhong::BlinnPhong(const char* diffuseMap, const char* normalMap)
        : IMaterial(make_unique<AShader>(BuiltinShaders::BlinnPhong)),
          m_DiffuseMapSource(diffuseMap), m_NormalMapSource(normalMap) {}

    void BlinnPhong::Initialize() {
        if (m_DiffuseMapSource) {
            glGenTextures(1, &m_DiffuseMap);
            glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,
                            GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            u8* data =
              stbi_load(m_DiffuseMapSource, &width, &height, &nrChannels, 0);
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

        if (m_NormalMapSource) {
            glGenTextures(1, &m_NormalMap);
            glBindTexture(GL_TEXTURE_2D, m_NormalMap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,
                            GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            u8* data =
              stbi_load(m_NormalMapSource, &width, &height, &nrChannels, 0);
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
    }

    void BlinnPhong::Use() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_NormalMap);
        m_Shader->Use();
    }

    void BlinnPhong::UpdateUniforms(FSceneContext& sceneContext,
                                    ACamera* camera) {
        if (sceneContext.Sun.get()) {
            m_Shader->SetVec3("u_LightColor", sceneContext.Sun->GetColor());
            m_Shader->SetVec3("u_LightPosition",
                              sceneContext.Sun->GetDirection());
            m_Shader->SetFloat("u_LightStrength",
                               sceneContext.Sun->GetStrength());
        }

        m_Shader->SetVec3("u_ViewPosition",
                          camera->GetTransform()->GetPosition());
        m_Shader->SetVec3("u_ObjectColor", m_DiffuseColor);
        m_Shader->SetInt("u_DiffuseMap", 0);
        m_Shader->SetInt("u_NormalMap", 1);
        m_Shader->SetInt("u_ShadowMap", 2);
        m_Shader->SetFloat("u_UV_Scale", m_UVScale);
    }

    void BlinnPhong::Destroy() {}

    Unlit::Unlit() : IMaterial(make_unique<AShader>(BuiltinShaders::Unlit)) {}

    void Unlit::Initialize() {}

    void Unlit::Use() {
        m_Shader->Use();
    }

    void Unlit::UpdateUniforms(FSceneContext& sceneContext, ACamera* camera) {
        m_Shader->SetVec3("u_ObjectColor", m_Color);
    }

    void Unlit::Destroy() {}

}  // namespace Materials