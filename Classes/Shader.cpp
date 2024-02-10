#include "Shader.h"

#include <fstream>
#include <sstream>
#include <result.hpp>
#include <cstring>
#include <exception>

#include "glad/glad.h"

static auto CheckCompileErrors(
    uint32_t shader) -> cpp::result<void, const char*> {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        return cpp::fail(infoLog);
    }

    return {};
}

static auto
ReadShader(
    const char* path) noexcept -> cpp::result<
    const ShaderSource, ReadShaderError> {
    const auto VERTEX_START   = "#define VERTEX\n";
    const auto VERTEX_END     = "#undef VERTEX\n";
    const auto FRAGMENT_START = "#define FRAGMENT\n";
    const auto FRAGMENT_END   = "#undef FRAGMENT\n";

    std::ifstream file(path, std::ios::in);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string strSource = buffer.str();

    // Check syntax is correct
    // =======================
    size_t vertStart = strSource.find(VERTEX_START);
    if (vertStart == std::string::npos) {
        return cpp::fail(ReadShaderError::READ_SHADER_ERROR_SYNTAX_VS);
    }
    vertStart += strlen(VERTEX_START);

    size_t vertEnd = strSource.find(VERTEX_END);
    if (vertEnd == std::string::npos) {
        return cpp::fail(ReadShaderError::READ_SHADER_ERROR_SYNTAX_VS);
    }
    vertEnd = vertEnd - vertStart;

    size_t fragStart = strSource.find(FRAGMENT_START);
    if (fragStart == std::string::npos) {
        return cpp::fail(ReadShaderError::READ_SHADER_ERROR_SYNTAX_FS);
    }
    fragStart += strlen(FRAGMENT_START);

    size_t fragEnd = strSource.find(FRAGMENT_END);
    if (fragEnd == std::string::npos) {
        return cpp::fail(ReadShaderError::READ_SHADER_ERROR_SYNTAX_FS);
    }
    fragEnd = fragEnd - fragStart;

    std::string vertexCode   = strSource.substr(vertStart, vertEnd);
    std::string fragmentCode = strSource.substr(fragStart, fragEnd);

    return ShaderSource{
        vertexCode,
        fragmentCode,
    };
}

void Shader::CompileShaders(const ShaderSource& sources) {
    const char* vertexCode   = sources.m_Vertex.c_str();
    const char* fragmentCode = sources.m_Fragment.c_str();

    uint32_t vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,
                   1,
                   &vertexCode,
                   nullptr);
    glCompileShader(vertexShader);
    const auto vertCompileResult = CheckCompileErrors(vertexShader);
    if (vertCompileResult.has_error()) {
        fprintf(stderr,
                "ERROR::SHADER::VS::COMPILE - %s\n",
                vertCompileResult.error());
        return;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,
                   1,
                   &fragmentCode,
                   nullptr);
    glCompileShader(fragmentShader);
    const auto fragCompileResult = CheckCompileErrors(fragmentShader);
    if (fragCompileResult.has_error()) {
        fprintf(stderr,
                "ERROR::SHADER::FS::COMPILE - %s\n",
                fragCompileResult.error());
        return;
    }

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);
    glLinkProgram(m_ShaderProgram);
    const auto linkShaderResult = CheckCompileErrors(m_ShaderProgram);
    if (linkShaderResult.has_error()) {
        fprintf(stderr,
                "ERROR::SHADER::LINK - %s\n",
                linkShaderResult.error());
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(const char* path) {
    const auto sources = ReadShader(path);
    if (sources.has_error()) {
        fprintf(stderr, "ERROR::SHADER::READ - %d\n", (int)sources.error());
        throw std::exception();
    }

    CompileShaders(sources.value());
}

void Shader::Use() {
    glUseProgram(m_ShaderProgram);
}

void Shader::Destroy() {
    glDeleteProgram(m_ShaderProgram);
}

void Shader::SetBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                (int)value);
}

void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                 1,
                 &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                 1,
                 &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                 1,
                 &value[0]);
}

void Shader::SetVec4(const std::string& name,
                     float x,
                     float y,
                     float z,
                     float w) const {
    glUniform4f(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                x,
                y,
                z,
                w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}
