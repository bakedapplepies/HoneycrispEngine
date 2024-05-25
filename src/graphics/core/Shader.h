#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

// TODO: maybe add shader types from constants.h
class Shader
{
private:
    GLuint m_shaderID;
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;

private:
    std::string parseShader(std::string_view path);
    GLint getUniformLocation(const std::string& name) const;

public:
    Shader() = default;
    Shader(
        const FileSystem::Path& vertexFile,
        const FileSystem::Path& fragmentFile,
        const FileSystem::Path& geometryFile = {""}
    );
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    ~Shader();
    
    GLuint getID() const;
    void Use() const;

    void setIntUnf(const std::string& name, int value) const;
    void setFloatUnf(const std::string& name, float value) const;
    void setMat4Unf(const std::string& name, const glm::mat4& matrix) const;
    void setMat3Unf(const std::string& name, const glm::mat3& matrix) const;
    void setVec2Unf(const std::string& name, const glm::vec2& vector) const;
    void setVec3Unf(const std::string& name, const glm::vec3& vector) const;
};

HNCRSP_NAMESPACE_END