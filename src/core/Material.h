#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Texture2D.h"
#include "src/opengl/Shader.h"


HNCRSP_NAMESPACE_START

class Material
{
private:
    std::shared_ptr<Shader> m_shader = nullptr;

    Texture2D* m_albedo = nullptr;
    Texture2D* m_roughness = nullptr;
    Texture2D* m_ao = nullptr;
    Texture2D* m_normal = nullptr;
    Texture2D* m_specular = nullptr;

    float m_shininess = 128.0f;

public:
    Material(std::shared_ptr<Shader> shader);
    Material(const Material& other);
    Material& operator=(const Material& other) noexcept;
    Material(Material&& other);
    Material& operator=(Material&& other) noexcept;
    ~Material();

    void setAlbedoMap(const FileSystem::Path& path);
    void setAlbedoMap(Texture2D& textureObj);
    void setRoughnessMap(const FileSystem::Path& path);
    void setRoughnessMap(Texture2D& textureObj);
    void setAoMap(const FileSystem::Path& path);
    void setAoMap(Texture2D& textureObj);
    void setNormalMap(const FileSystem::Path& path);
    void setNormalMap(Texture2D& textureObj);
    void setSpecularMap(const FileSystem::Path& path);
    void setSpecularMap(Texture2D& textureObj);

    void setShininess(float shininess);

    std::shared_ptr<Shader>  getShader() const;
    void setShader(std::shared_ptr<Shader> newShader);
    const Texture2D* getAlbedoMap();
    const Texture2D* getRoughnessMap();
    const Texture2D* getAoMap();
    const Texture2D* getNormalMap();
    const Texture2D* getSpecularMap();
    float getShininess() const;

    // void bindMaterial(Shader* shader);
};

HNCRSP_NAMESPACE_END