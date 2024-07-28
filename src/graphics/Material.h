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
    ~Material() = default;  // Resources are already managed by managers

    void SetAlbedoMap(const FileSystem::Path& path);
    void SetAlbedoMap(const Texture2D* textureObj);
    void SetRoughnessMap(const FileSystem::Path& path);
    void SetRoughnessMap(const Texture2D* textureObj);
    void SetAoMap(const FileSystem::Path& path);
    void SetAoMap(const Texture2D* textureObj);
    void SetNormalMap(const FileSystem::Path& path);
    void SetNormalMap(const Texture2D* textureObj);
    void SetSpecularMap(const FileSystem::Path& path);
    void SetSpecularMap(const Texture2D* textureObj);

    void SetShininess(float shininess);

    const Shader* getShader() const;
    void SetShader(const Shader* newShader);
    const Texture2D* GetAlbedoMap();
    const Texture2D* GetRoughnessMap();
    const Texture2D* GetAoMap();
    const Texture2D* GetNormalMap();
    const Texture2D* GetSpecularMap();
    float GetShininess() const;

private:
    void _UpdateTextureUniforms() const;  // TODO: Currently unused
};

HNCRSP_NAMESPACE_END