//
// Created by jr on 2/16/24.
//

#include "Material.h"

namespace Materials {
    BlinnPhong::BlinnPhong()
        : IMaterial(make_unique<AShader>(BuiltinShaders::BlinnPhong)) {}

    void BlinnPhong::Initialize() {}

    void BlinnPhong::Use() {
        m_Shader->Use();
    }

    void BlinnPhong::UpdateUniforms() {
        m_Shader->SetVec3("u_ObjectColor", m_DiffuseColor);
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