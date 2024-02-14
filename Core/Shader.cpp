#include "Shader.h"

#include "Utilities.inl"
#include "Shaders/ShaderImports.inl"

#include <fstream>
#include <result.hpp>
#include <string>
#include <exception>
#include <regex>

#include "glad/glad.h"

static cpp::result<void, const char*> CheckCompileErrors(uint32_t shader) {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        return cpp::fail(infoLog);
    }

    return {};
}

static cpp::result<const FShaderSource, EReadShaderError>
ReadShaderFromFile(const char* path) noexcept {
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

static cpp::result<const FShaderSource, EReadShaderError>
ReadShaderFromSource(const string& source) noexcept {
    const auto VERTEX_START   = "#define VERTEX\n";
    const auto VERTEX_END     = "#undef VERTEX\n";
    const auto FRAGMENT_START = "#define FRAGMENT\n";
    const auto FRAGMENT_END   = "#undef FRAGMENT\n";

    // Check syntax is correct
    // =======================
    size_t vertStart = source.find(VERTEX_START);
    if (vertStart == string::npos) {
        return cpp::fail(EReadShaderError::READ_SHADER_ERROR_SYNTAX_VS);
    }
    vertStart += strlen(VERTEX_START);

    size_t vertEnd = source.find(VERTEX_END);
    if (vertEnd == string::npos) {
        return cpp::fail(EReadShaderError::READ_SHADER_ERROR_SYNTAX_VS);
    }
    vertEnd = vertEnd - vertStart;

    size_t fragStart = source.find(FRAGMENT_START);
    if (fragStart == string::npos) {
        return cpp::fail(EReadShaderError::READ_SHADER_ERROR_SYNTAX_FS);
    }
    fragStart += strlen(FRAGMENT_START);

    size_t fragEnd = source.find(FRAGMENT_END);
    if (fragEnd == string::npos) {
        return cpp::fail(EReadShaderError::READ_SHADER_ERROR_SYNTAX_FS);
    }
    fragEnd = fragEnd - fragStart;

    const string vertexCode   = source.substr(vertStart, vertEnd);
    const string fragmentCode = source.substr(fragStart, fragEnd);

    return FShaderSource {
      vertexCode,
      fragmentCode,
    };
}

FShaderSource AShader::Preprocess(const FShaderSource& sources) {
    std::string vertexCode   = sources.m_Vertex.c_str();
    std::string fragmentCode = sources.m_Fragment.c_str();
    std::regex importRgx("import ([a-zA-Z]+);");

    // 1. Process vertex importsW
    std::smatch vertexMatches;
    for (auto it = std::sregex_iterator(vertexCode.begin(),
                                        vertexCode.end(),
                                        importRgx);
         it != std::sregex_iterator();
         ++it) {
        std::smatch match;
        match = *it;

        const auto importLen  = match.str(0).length();
        const auto importName = match.str(1);
        const auto importPos  = match.position(0);

        printf("'%s'\n", importName.c_str());

        if (auto search = ShaderImports::ImportMap.find(importName.c_str());
            search != ShaderImports::ImportMap.end()) {
            printf("%s\n", search->first);
            vertexCode.replace(importPos, importLen, search->second.c_str());
        }
    }
    printf("%s\n", vertexCode.c_str());

    // 2. Process fragment imports

    return {vertexCode.c_str(), fragmentCode.c_str()};
}

void AShader::CompileShaders(const FShaderSource& sources) {
    const auto vertexCode   = sources.m_Vertex.c_str();
    const auto fragmentCode = sources.m_Fragment.c_str();

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
    const auto sources = ReadShaderFromFile(path);
    if (sources.has_error()) {
        fprintf(stderr,
                "ERROR::SHADER::READ - %d\n",
                static_cast<int>(sources.error()));
        throw std::exception();
    }

    const auto processedSources =
      sources.value(); /*Preprocess(sources.value());*/
    CompileShaders(processedSources);
}

AShader::AShader(const string& source) {
    const auto sources = ReadShaderFromSource(source);
    if (sources.has_error()) {
        fprintf(stderr,
                "ERROR::SHADER::READ - %d\n",
                static_cast<int>(sources.error()));
        throw std::exception();
    }

    const auto processedSources =
      sources.value(); /*Preprocess(sources.value());*/
    CompileShaders(processedSources);
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