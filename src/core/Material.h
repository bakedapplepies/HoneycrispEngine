#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Texture2D.h"
#include "src/opengl/Shader.h"


HNCRSP_NAMESPACE_START

class Material
{
private:
    std::shared_ptr<Shader> m_shader;

    Texture2D* m_albedo;
    Texture2D* m_roughness;
    Texture2D* m_ao;
    Texture2D* m_normal;
    Texture2D* m_specular;

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
    Texture2D const* getAlbedoMap();
    Texture2D const* getRoughnessMap();
    Texture2D const* getAoMap();
    Texture2D const* getNormalMap();
    Texture2D const* getSpecularMap();
    float getShininess() const;

    // void bindMaterial(Shader* shader);
};

HNCRSP_NAMESPACE_END