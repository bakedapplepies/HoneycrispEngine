#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/Texture2D.h"
#include "api/graphics/core/Shader.h"

HNCRSP_NAMESPACE_START

class Material
{
private:
    const Shader* m_shader = nullptr;

    const Texture2D* m_albedo = nullptr;
    const Texture2D* m_roughness = nullptr;
    const Texture2D* m_ao = nullptr;
    const Texture2D* m_normal = nullptr;
    const Texture2D* m_specular = nullptr;

    float m_shininess = 64.0f;
    bool m_isOpaque = true;

public:
    Material(const Shader* shader);
    Material(const Material& other);
    Material& operator=(const Material& other) noexcept;
    Material(Material&& other);
    Material& operator=(Material&& other) noexcept;
    ~Material() = default;  // Resources are already managed by managers

    void SetShader(const Shader* new_shader);
    void SetAlbedoMap(const FileSystem::Path& path);
    void SetAlbedoMap(const Texture2D* textureObj);
    void SetRoughnessMap(const FileSystem::Path& path);
    void SetRoughnessMap(const Texture2D* texture_obj);
    void SetAoMap(const FileSystem::Path& path);
    void SetAoMap(const Texture2D* texture_obj);
    void SetNormalMap(const FileSystem::Path& path);
    void SetNormalMap(const Texture2D* texture_obj);
    void SetSpecularMap(const FileSystem::Path& path);
    void SetSpecularMap(const Texture2D* texture_obj);

    void SetShininess(float shininess);
    void SetIsOpaque(bool is_opaque);

    inline const Shader* GetShader() const
    {
        return m_shader;
    }
    
    inline const Texture2D* GetAlbedoMap() const
    {
        return m_albedo;
    }
    
    inline const Texture2D* GetRoughnessMap() const
    {
        return m_roughness;
    }
    
    inline const Texture2D* GetAoMap() const
    {
        return m_ao;
    }
    
    inline const Texture2D* GetNormalMap() const
    {
        return m_normal;
    }
    
    inline const Texture2D* GetSpecularMap() const
    {
        return m_specular;
    }
    
    inline float GetShininess() const
    {
        return m_shininess;
    }
    
    inline bool IsOpaque() const
    {
        return m_isOpaque;
    }

private:
    void _UpdateTextureUniforms() const;  // TODO: Currently unused
};

HNCRSP_NODISCARD std::shared_ptr<Material> CreateMaterial(const Shader* shader);

HNCRSP_NAMESPACE_END