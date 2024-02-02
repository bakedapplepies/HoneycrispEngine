#pragma once

#include "src/pch/pch.h"
#include "src/renderer/Texture2D.h"


HNCRSP_NAMESPACE_START

class Material
{
private:
    Texture2D m_albedo;
    Texture2D m_roughness;
    Texture2D m_ao;
    Texture2D m_normal;
    Texture2D m_specular;

    float m_shininess;

public:
    Material() = default;
    ~Material();

    void setAlbedoMap(const FileSystem::Path& path);
    void setRoughnessMap(const FileSystem::Path& path);
    void setAoMap(const FileSystem::Path& path);
    void setNormalMap(const FileSystem::Path& path);
    void setSpecularMap(const FileSystem::Path& path);
};

HNCRSP_NAMESPACE_END