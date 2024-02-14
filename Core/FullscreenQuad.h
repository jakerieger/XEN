//
// Created by jr on 2/13/24.
//

#pragma once
#include <Shader.h>

/**
 * \brief Draws a fullscreen quad for processing render textures
 */
class AFullscreenQuad {
public:
    AFullscreenQuad() = default;
    void Initialize(const string& shaderSrc);
    void Render() const;
    void Update(float deltaTime);
    void Destroy() const;

private:
    AShader* m_Shader = nullptr;
    uint32_t VBO;
    uint32_t EBO;
    uint32_t VAO;
};