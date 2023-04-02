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

    void setFloatUniform(const std::string& name, float value) const;
    void setMatrixfvUniform(const std::string& name, glm::mat4 matrix) const;
};