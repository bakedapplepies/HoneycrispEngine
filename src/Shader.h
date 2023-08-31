#pragma once

#include "pch/pch.h"


class Shader
{
private:
    GLuint m_shaderID;
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;
    static std::vector<Shader*> sm_shaderRefs;

private:
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

public:
    Shader();
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    ~Shader();

    std::string parseShader(const std::string& path);
    
    GLuint getID() const;
    void Use() const;

    GLint getUniformLocation(const std::string& name) const;
    void setIntUniform(const std::string& name, int value) const;
    void setFloatUniform(const std::string& name, float value) const;
    void setMatrix4Uniform(const std::string& name, const glm::mat4& matrix) const;
    void setMatrix3Uniform(const std::string& name, const glm::mat3& matrix) const;
    void setVector3Uniform(const std::string& name, const glm::vec3& vector) const;
};