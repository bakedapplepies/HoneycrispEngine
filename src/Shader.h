#pragma once

#include "pch/pch.h"


class Shader {
private:
    GLuint m_shaderID;

public:
    std::string ParseShader(const std::string& path);
    void CreateShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    
    GLuint getID() const;
    
    void Use() const;
    void Delete() const;

    void setFloatUniform(const char* name, float value) const;
    void setMatrix4Uniform(const char* name, const glm::mat4& matrix) const;
    void setMatrix3Uniform(const char* name, const glm::mat3& matrix) const;
    void setVector3Uniform(const char* name, const glm::vec3& vector) const;
};