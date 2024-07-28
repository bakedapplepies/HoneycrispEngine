#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

// TODO: maybe add shader types from constants.h
class Shader
{
private:
    GLuint m_shaderID;
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;

private:
    std::string _ParseShader(std::string_view path);
    GLint _GetUniformLocation(const std::string& name) const;

public:
    Shader() = default;
    Shader(
        const FileSystem::Path& vertexFile,
        const FileSystem::Path& fragmentFile,
        const FileSystem::Path& geometryFile = {""}
    );
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader(Shader&& other) = delete;
    Shader& operator=(Shader&& other) = delete;
    ~Shader();
    
    GLuint GetID() const;
    
    inline void Use() const
    {
        GLCall(glUseProgram(m_shaderID));
    }

    inline void SetIntUnf(const std::string& name, int value) const
    {
        GLCall(glProgramUniform1i(m_shaderID, _GetUniformLocation(name), value));
    }

    inline void SetFloatUnf(const std::string& name, float value) const
    {
        GLCall(glProgramUniform1f(m_shaderID, _GetUniformLocation(name), value));
    }

    inline void SetMat4Unf(const std::string& name, const glm::mat4& matrix) const
    {
        GLCall(glProgramUniformMatrix4fv(
            m_shaderID,
            _GetUniformLocation(name),
            1,
            GL_FALSE,
            glm::value_ptr(matrix)
        ));
    }

    inline void SetMat3Unf(const std::string& name, const glm::mat3& matrix) const
    {
        GLCall(glProgramUniformMatrix3fv(
            m_shaderID,
            _GetUniformLocation(name),
            1,
            GL_FALSE,
            glm::value_ptr(matrix)
        ));
    }

    inline void SetVec2Unf(const std::string& name, const glm::vec2& vector) const
    {
        GLCall(glProgramUniform2fv(
            m_shaderID,
            _GetUniformLocation(name),
            1,
            glm::value_ptr(vector)
        ));
    }

    inline void SetUVec2Unf(const std::string& name, const glm::uvec2& vector) const
    {
        GLCall(glProgramUniform2uiv(
            m_shaderID,
            _GetUniformLocation(name),
            1,
            glm::value_ptr(vector)
        ));
    }

    inline void SetVec3Unf(const std::string& name, const glm::vec3& vector) const
    {
        GLCall(glProgramUniform3fv(
            m_shaderID,
            _GetUniformLocation(name),
            1,
            glm::value_ptr(vector)
        ));
    }
};

HNCRSP_NAMESPACE_END