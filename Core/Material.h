//
// Created by jr on 2/16/24.
//

#pragma once

#include "Camera.h"
#include "STL.h"
#include <Shader.h>

struct FSceneContext;
class ACamera;

/**
 * \brief Abstract material class for implementing
 * materials
 */
class IMaterial {
public:
    explicit IMaterial(eastl::unique_ptr<AShader> shader)
        : m_Shader(move(shader)) {}
    virtual ~IMaterial() = default;

    virtual void Initialize()                    = 0;
    virtual void Destroy()                       = 0;
    virtual void Use()                           = 0;
    virtual void UpdateUniforms(FSceneContext& sceneContext,
                                ACamera* camera) = 0;

    AShader* GetShader() const {
        return m_Shader.get();
    }

    template<typename T>
    T* Cast() {
        return dynamic_cast<T*>(this);
    }

    template<typename T>
    static eastl::unique_ptr<IMaterial> Create() {
        return eastl::unique_ptr<IMaterial>(new T);
    }

protected:
    eastl::unique_ptr<AShader> m_Shader;
};

namespace Materials {
    class BlinnPhong final : public IMaterial {
    public:
        BlinnPhong();
        void Initialize() override;
        void Destroy() override;
        void Use() override;
        void UpdateUniforms(FSceneContext& sceneContext,
                            ACamera* camera) override;

        void SetColor(const glm::vec3& color) {
            m_DiffuseColor = color;
        }

        void SetUVScale(const float scale) {
            m_UVScale = scale;
        }

    private:
        glm::vec3 m_DiffuseColor  = {0.5f, 0.5f, 0.5f};
        glm::vec3 m_SpecularColor = {1.f, 1.f, 1.f};
        float m_SpecularStrength  = 1.f;
        u32 m_DiffuseTexture      = 0;
        float m_UVScale           = 1.f;
    };

    class SkyDome final : public IMaterial {
    public:
        SkyDome();
        void Initialize() override;
        void Destroy() override;
        void Use() override;
        void UpdateUniforms(FSceneContext& sceneContext,
                            ACamera* camera) override;

        void SetUVScale(const float scale) {
            m_UVScale = scale;
        }

    private:
        u32 m_Texture   = 0;
        float m_UVScale = 1.f;
    };

    class Unlit final : public IMaterial {
    public:
        Unlit();
        void Initialize() override;
        void Destroy() override;
        void Use() override;
        void UpdateUniforms(FSceneContext& sceneContext,
                            ACamera* camera) override;

    private:
        glm::vec3 m_Color = {1.f, 0.f, 0.f};
    };

}  // namespace Materials