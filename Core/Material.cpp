//
// Created by jr on 2/16/24.
//

#include "Material.h"

#include "Camera.h"
#include "GraphicsContext.h"

namespace Materials {
    void BlinnPhong::Initialize() {
        m_Shader = new AShader(BuiltinShaders::BlinnPhong);
        m_Shader->Use();
        m_Shader->SetMat4(
          "u_Projection",
          ACamera::GetProjectionMatrix(45.f, Graphics::GetWindowAspect()));
    }

    void BlinnPhong::Draw() {}

    void BlinnPhong::Update() {
        m_Shader->Use();
    }

    void BlinnPhong::Destroy() {
        delete m_Shader;
    }
}  // namespace Materials