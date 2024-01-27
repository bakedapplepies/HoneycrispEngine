#pragma once

#include "src/pch/pch.h"
#include "../utils/utils.h"


HNCRSP_NAMESPACE_START

class Shader
{
private:
    GLuint m_shaderID;
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;

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

    std::string parseShader(const std::string_view& path);
    
    GLuint getID() const;
    void Use() const;

    GLint getUniformLocation(const std::string& name) const;
    void setIntUnf(const std::string& name, int value) const;
    void setFloatUnf(const std::string& name, float value) const;
    void setMat4Unf(const std::string& name, const glm::mat4& matrix) const;
    void setMat3Unf(const std::string& name, const glm::mat3& matrix) const;
    void setVec3Unf(const std::string& name, const glm::vec3& vector) const;
};

HNCRSP_NAMESPACE_END