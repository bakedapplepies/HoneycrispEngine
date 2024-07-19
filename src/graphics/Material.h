#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/graphics/core/Texture2D.h"
#include "src/graphics/core/Shader.h"


HNCRSP_NAMESPACE_START

class Material
{
private:
    const Shader* m_shader;

    const Texture2D* m_albedo;
    const Texture2D* m_roughness;
    const Texture2D* m_ao;
    const Texture2D* m_normal;
    const Texture2D* m_specular;

    float m_shininess;

public:
    Material(const Shader* shader);
    Material(const Material& other);
    Material& operator=(const Material& other) noexcept;
    Material(Material&& other);
    Material& operator=(Material&& other) noexcept;
    ~Material();

    void setAlbedoMap(const FileSystem::Path& path);
    void setAlbedoMap(const Texture2D* textureObj);
    void setRoughnessMap(const FileSystem::Path& path);
    void setRoughnessMap(const Texture2D* textureObj);
    void setAoMap(const FileSystem::Path& path);
    void setAoMap(const Texture2D* textureObj);
    void setNormalMap(const FileSystem::Path& path);
    void setNormalMap(const Texture2D* textureObj);
    void setSpecularMap(const FileSystem::Path& path);
    void setSpecularMap(const Texture2D* textureObj);

    void setShininess(float shininess);

    const Shader* getShader() const;
    void setShader(const Shader* newShader);
    const Texture2D* getAlbedoMap();
    const Texture2D* getRoughnessMap();
    const Texture2D* getAoMap();
    const Texture2D* getNormalMap();
    const Texture2D* getSpecularMap();
    float getShininess() const;

private:
    void UpdateTextureUniforms() const;
};

HNCRSP_NAMESPACE_END