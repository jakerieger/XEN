﻿#pragma once

#include "Types.h"
#include <glm/glm.hpp>
#include "STL.h"

struct FShaderSource {
    eastl::string m_Vertex;
    eastl::string m_Fragment;
};

enum class EReadShaderError {
    READ_SHADER_IO,
    READ_SHADER_ERROR_SYNTAX_VS,
    READ_SHADER_ERROR_SYNTAX_FS,
    READ_SHADER_ERROR_COMPILE_VS,
    READ_SHADER_ERROR_COMPILE_FS,
    READ_SHADER_ERROR_LINK,
};

/**
 * \brief Represents a vertex+fragment shader program
 */
class AShader {
public:
    explicit AShader(const char* path);
    explicit AShader(const eastl::string& source);
    explicit AShader(const FShaderSource& sources);
    ~AShader() = default;

    void Use() const;
    void Destroy() const;

    //=================//
    // Uniform Setters //
    //=================//
    void SetBool(const eastl::string& name, bool value) const;
    void SetInt(const eastl::string& name, int value) const;
    void SetFloat(const eastl::string& name, float value) const;
    void SetVec2(const eastl::string& name, const glm::vec2& value) const;
    void SetVec2(const eastl::string& name, float x, float y) const;
    void SetVec3(const eastl::string& name, const glm::vec3& value) const;
    void SetVec3(const eastl::string& name, float x, float y, float z) const;
    void SetVec4(const eastl::string& name, const glm::vec4& value) const;
    void SetVec4(
      const eastl::string& name, float x, float y, float z, float w) const;
    void SetMat2(const eastl::string& name, const glm::mat2& mat) const;
    void SetMat3(const eastl::string& name, const glm::mat3& mat) const;
    void SetMat4(const eastl::string& name, const glm::mat4& mat) const;

private:
    void CompileShaders(const FShaderSource& sources);
    u32 m_ShaderProgram {};
};

namespace BuiltinShaders {
    const inline eastl::string Quad =
#include "Shaders/Quad.glsl"
      ;
    const inline eastl::string Text =
#include "Shaders/Text.glsl"
      ;
    const inline eastl::string Unlit =
#include "Shaders/Unlit.glsl"
      ;
    const inline eastl::string BlinnPhong =
#include "Shaders/BlinnPhong.glsl"
      ;
    const inline eastl::string SkyDome =
#include "Shaders/SkyDome.glsl"
      ;
    const inline eastl::string Skybox =
#include "Shaders/Skybox.glsl"
      ;
}  // namespace BuiltinShaders
