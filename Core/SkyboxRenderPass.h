//
// Created by conta on 2/26/2024.
//

#pragma once

#include "Interfaces/RenderPass.h"
#include "Skybox.h"
#include "STL.h"

class ACamera;

class ASkyboxRenderPass final : public IRenderPass {
public:
    void Init() override;
    void Render(ACamera* camera) const;
    void Cleanup() override;

private:
    eastl::unique_ptr<ASkybox> m_Skybox;
};

inline void ASkyboxRenderPass::Init() {
    m_Skybox = eastl::make_unique<ASkybox>();
    m_Skybox->Init();
}

inline void ASkyboxRenderPass::Render(ACamera* camera) const {
    if (m_Enabled) {
        m_Skybox->Draw(camera);
    }
}

inline void ASkyboxRenderPass::Cleanup() {
    m_Skybox.release();
}