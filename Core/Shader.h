#pragma once

#include <stdint.h>
#include <string>
#include <glm/glm.hpp>

struct FShaderSource {
    std::string m_Vertex;
    std::string m_Fragment;
};

enum class EReadShaderError {
    READ_SHADER_ERROR_SYNTAX_VS,
    READ_SHADER_ERROR_SYNTAX_FS,
    READ_SHADER_ERROR_COMPILE_VS,
    READ_SHADER_ERROR_COMPILE_FS,
    READ_SHADER_ERROR_LINK,
};

class AShader {
public:
    explicit AShader(const char* path);
    ~AShader() = default;

    void Use();

    void Destroy();

    void SetBool(const std::string& name, bool value) const;

    void SetInt(const std::string& name, int value) const;

    void SetFloat(const std::string& name, float value) const;

    void SetVec2(const std::string& name, const glm::vec2& value) const;

    void SetVec2(const std::string& name, float x, float y) const;

    void SetVec3(const std::string& name, const glm::vec3& value) const;

    void SetVec3(const std::string& name, float x, float y, float z) const;

    void SetVec4(const std::string& name, const glm::vec4& value) const;

    void
    SetVec4(const std::string& name, float x, float y, float z, float w) const;

    void SetMat2(const std::string& name, const glm::mat2& mat) const;

    void SetMat3(const std::string& name, const glm::mat3& mat) const;

    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    void CompileShaders(const FShaderSource& sources);
    uint32_t m_ShaderProgram;
};
