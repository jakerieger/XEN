//
// Created by conta on 2/21/2024.
//

#pragma once

#include "STL.h"

class AShader;
class ACamera;

struct FCubemap {
    eastl::string Right;
    eastl::string Left;
    eastl::string Top;
    eastl::string Bottom;
    eastl::string Front;
    eastl::string Back;
};

class ASkybox {
public:
    ASkybox();
    void Init();
    void Draw(ACamera* camera) const;
    void Destroy() const;

private:
    static u32 LoadCubemap(const eastl::vector<eastl::string>& faces);
    u32 m_CubemapTexture;
    eastl::unique_ptr<AShader> m_Shader;
    u32 m_VAO;
    u32 m_VBO;
};