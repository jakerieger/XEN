//
// Created by jr on 2/16/24.
//

#include "Material.h"

namespace Materials {
    void BlinnPhong::Initialize() {
        this->m_Shader = new AShader(BuiltinShaders::BlinnPhong);
    }

    void BlinnPhong::Use() {
        m_Shader->Use();
    }

    void BlinnPhong::UpdateUniforms() {
        m_Shader->SetVec3("u_ObjectColor", m_DiffuseColor);
    }

    void BlinnPhong::Destroy() {
        delete m_Shader;
    }
}  // namespace Materials