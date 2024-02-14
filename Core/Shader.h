#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include "STL.h"

struct FShaderSource {
    string m_Vertex;
    string m_Fragment;
};

enum class EReadShaderError {
    READ_SHADER_IO,
    READ_SHADER_ERROR_SYNTAX_VS,
    READ_SHADER_ERROR_SYNTAX_FS,
    READ_SHADER_ERROR_COMPILE_VS,
    READ_SHADER_ERROR_COMPILE_FS,
    READ_SHADER_ERROR_LINK,
};

class AShader {
public:
    explicit AShader(const char* path);
    explicit AShader(const string& source);
    ~AShader() = default;

    void Use() const;

    void Destroy() const;

    void SetBool(const string& name, bool value) const;

    void SetInt(const string& name, int value) const;

    void SetFloat(const string& name, float value) const;

    void SetVec2(const string& name, const glm::vec2& value) const;

    void SetVec2(const string& name, float x, float y) const;

    void SetVec3(const string& name, const glm::vec3& value) const;

    void SetVec3(const string& name, float x, float y, float z) const;

    void SetVec4(const string& name, const glm::vec4& value) const;

    void SetVec4(const string& name, float x, float y, float z, float w) const;

    void SetMat2(const string& name, const glm::mat2& mat) const;

    void SetMat3(const string& name, const glm::mat3& mat) const;

    void SetMat4(const string& name, const glm::mat4& mat) const;

private:
    static FShaderSource Preprocess(const FShaderSource& sources);
    void CompileShaders(const FShaderSource& sources);
    uint32_t m_ShaderProgram {};
};

namespace BuiltinShaders {
    const inline string Quad =
#include "Shaders/Quad.glsl"
      ;
    const inline string Text =
#include "Shaders/Text.glsl"
      ;
}  // namespace BuiltinShaders
