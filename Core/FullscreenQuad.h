//
// Created by jr on 2/13/24.
//

#pragma once

#include "Mesh.h"
#include "Shader.h"

/**
 * \brief Draws a fullscreen quad for processing render textures
 */
class AFullscreenQuad {
public:
    AFullscreenQuad() = default;
    void Initialize(const string& shaderSrc);
    void Render() const;
    void Update(float deltaTime) const;
    void Destroy() const;

private:
    AShader* m_Shader = nullptr;
    AMesh* m_Mesh     = nullptr;
};