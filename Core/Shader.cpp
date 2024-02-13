#include "Shader.h" g_Font

#include "Utilities.inl"

#include <fstream>
#include <sstream>
#include <result.hpp>
#include <cstring>
#include <exception>

#include "glad/glad.h"

static auto CheckCompileErrors(uint32_t shader)
  -> cpp::result<void, const char*> {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        return cpp::fail(infoLog);
    }

    return {};
}

static auto ReadShader(const char* path) noexcept
  -> cpp::result<const FShaderSource, EReadShaderError> {
    const auto VERTEX_START   = "#define VERTEX\n";
    const auto VERTEX_END     = "#undef VERTEX\n";
    const auto FRAGMENT_START = "#define FRAGMENT\n";
    const auto FRAGMENT_END   = "#undef FRAGMENT\n";

    const auto readShaderResult = Utilities::ReadFileToString(path);
    if (readShaderResult.has_error()) {
        return cpp::fail(EReadShaderError::READ_SHADER_IO);
    }
    const string& strSource = readShaderResult.value();

    // Check syntax is correct
    // =======================
    size_t vertStart = strSource.find(VERTEX_START);
    if (vertStart == string::npos) {
        return cpp::fail(EReadShaderError::READ_SHADER_ERROR_SYNTAX_VS);
    }
    vertStart += strlen(VERTEX_START);

    size_t vertEnd = strSource.find(VERTEX_END);
    if (vertEnd == string::npos) {
        return cpp::fail(EReadShaderError::READ_SHADER_ERROR_SYNTAX_VS);
    }
    vertEnd = vertEnd - vertStart;

    size_t fragStart = strSource.find(FRAGMENT_START);
    if (fragStart == string::npos) {
        return cpp::fail(EReadShaderError::READ_SHADER_ERROR_SYNTAX_FS);
    }
    fragStart += strlen(FRAGMENT_START);

    size_t fragEnd = strSource.find(FRAGMENT_END);
    if (fragEnd == string::npos) {
        return cpp::fail(EReadShaderError::READ_SHADER_ERROR_SYNTAX_FS);
    }
    fragEnd = fragEnd - fragStart;

    const string vertexCode   = strSource.substr(vertStart, vertEnd);
    const string fragmentCode = strSource.substr(fragStart, fragEnd);

    return FShaderSource {
      vertexCode,
      fragmentCode,
    };
}

void AShader::CompileShaders(const FShaderSource& sources) {
    const char* vertexCode   = sources.m_Vertex.c_str();
    const char* fragmentCode = sources.m_Fragment.c_str();

    const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);
    if (const auto vertCompileResult = CheckCompileErrors(vertexShader);
        vertCompileResult.has_error()) {
        fprintf(stderr,
                "ERROR::SHADER::VS::COMPILE - %s\n",
                vertCompileResult.error());
        return;
    }

    const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);
    if (const auto fragCompileResult = CheckCompileErrors(fragmentShader);
        fragCompileResult.has_error()) {
        fprintf(stderr,
                "ERROR::SHADER::FS::COMPILE - %s\n",
                fragCompileResult.error());
        return;
    }

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);
    glLinkProgram(m_ShaderProgram);
    if (const auto linkShaderResult = CheckCompileErrors(m_ShaderProgram);
        linkShaderResult.has_error()) {
        fprintf(stderr, "ERROR::SHADER::LINK - %s\n", linkShaderResult.error());
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

AShader::AShader(const char* path) {
    const auto sources = ReadShader(path);
    if (sources.has_error()) {
        fprintf(stderr,
                "ERROR::SHADER::READ - %d\n",
                static_cast<int>(sources.error()));
        throw std::exception();
    }

    CompileShaders(sources.value());
}

void AShader::Use() const {
    glUseProgram(m_ShaderProgram);
}

void AShader::Destroy() const {
    glDeleteProgram(m_ShaderProgram);
}

void AShader::SetBool(const string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                static_cast<int>(value));
}

void AShader::SetInt(const string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
}

void AShader::SetFloat(const string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
}

void AShader::SetVec2(const string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                 1,
                 &value[0]);
}

void AShader::SetVec2(const string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y);
}

void AShader::SetVec3(const string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                 1,
                 &value[0]);
}

void AShader::SetVec3(const string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y, z);
}

void AShader::SetVec4(const string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                 1,
                 &value[0]);
}

void AShader::SetVec4(
  const string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                x,
                y,
                z,
                w);
}

void AShader::SetMat2(const string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}

void AShader::SetMat3(const string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}

void AShader::SetMat4(const string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}
