//
// Created by jr on 2/16/24.
//

#pragma once

#include "STL.h"
#include <Shader.h>

/**
 * \brief Abstract material class for implementing
 * materials
 */
class IMaterial {
public:
    virtual ~IMaterial() = default;

    virtual void Initialize()     = 0;
    virtual void Destroy()        = 0;
    virtual void Use()            = 0;
    virtual void UpdateUniforms() = 0;

    AShader& GetShader() const {
        return *m_Shader;
    }

protected:
    AShader* m_Shader = nullptr;
};

namespace Materials {
    class BlinnPhong final : public IMaterial {
    public:
        void Initialize() override;
        void Destroy() override;
        void Use() override;
        void UpdateUniforms() override;

    private:
        glm::vec3 m_DiffuseColor  = {0.5f, 0.5f, 0.5f};
        glm::vec3 m_SpecularColor = {1.f, 1.f, 1.f};
        float m_SpecularStrength  = 1.f;
    };
}  // namespace Materials