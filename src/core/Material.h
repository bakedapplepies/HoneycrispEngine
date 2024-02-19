#pragma once

#include "src/pch/pch.h"
#include "src/renderer/Texture2D.h"
#include "src/renderer/Shader.h"


HNCRSP_NAMESPACE_START

class Material
{
private:
    std::shared_ptr<Shader> m_shader;

    std::shared_ptr<Texture2D> m_albedo;
    std::shared_ptr<Texture2D> m_roughness;
    std::shared_ptr<Texture2D> m_ao;
    std::shared_ptr<Texture2D> m_normal;
    std::shared_ptr<Texture2D> m_specular;

    float m_shininess = 128.0f;

public:
    Material(std::shared_ptr<Shader> shader);
    Material(const Material& other);
    Material& operator=(const Material& other) noexcept;
    Material(Material&& other);
    Material& operator=(Material&& other) noexcept;
    ~Material();

    void setAlbedoMap(const FileSystem::Path& path);
    void setRoughnessMap(const FileSystem::Path& path);
    void setAoMap(const FileSystem::Path& path);
    void setNormalMap(const FileSystem::Path& path);
    void setSpecularMap(const FileSystem::Path& path);
    void setShininess(float shininess);

    std::weak_ptr<Texture2D> getAlbedoMap() const;
    std::weak_ptr<Texture2D> getRoughnessMap() const;
    std::weak_ptr<Texture2D> getAoMap() const;
    std::weak_ptr<Texture2D> getNormalMap() const;
    std::weak_ptr<Texture2D> getSpecularMap() const;
    float getShininess() const;

    // void bindMaterial(Shader* shader);
};

HNCRSP_NAMESPACE_END