#include "Material.h"
#include "src/core/Texture2DManager.h"


HNCRSP_NAMESPACE_START

Material::Material(std::shared_ptr<Shader> shader)
{
    m_shader = shader;
}

Material::~Material()
{
}

Material::Material(const Material& other)
{
    m_shader = other.m_shader;
    m_albedo = other.m_albedo;
    m_roughness = other.m_roughness;
    m_ao = other.m_ao;
    m_normal = other.m_normal;
    m_specular = other.m_specular;
    m_shininess = other.m_shininess;
}

Material& Material::operator=(const Material& other) noexcept
{
    m_shader = other.m_shader;
    m_albedo = other.m_albedo;
    m_roughness = other.m_roughness;
    m_ao = other.m_ao;
    m_normal = other.m_normal;
    m_specular = other.m_specular;
    m_shininess = other.m_shininess;

    return *this;
}

Material::Material(Material&& other)
{
    m_shader = std::move(other.m_shader);
    m_albedo = std::move(other.m_albedo);
    m_roughness = std::move(other.m_roughness);
    m_ao = std::move(other.m_ao);
    m_normal = std::move(other.m_normal);
    m_specular = std::move(other.m_specular);
    m_shininess = std::move(other.m_shininess);

    other.m_shininess = 0.0f;
}

Material& Material::operator=(Material&& other) noexcept
{
    m_shader = std::move(other.m_shader);
    m_albedo = std::move(other.m_albedo);
    m_roughness = std::move(other.m_roughness);
    m_ao = std::move(other.m_ao);
    m_normal = std::move(other.m_normal);
    m_specular = std::move(other.m_specular);
    m_shininess = std::move(other.m_shininess);

    other.m_shininess = 0.0f;

    return *this;
}

void Material::setAlbedoMap(const FileSystem::Path& path)
{
    m_albedo = &g_Texture2DManager.getTexture2D(path, ETextureType::ALBEDO, 1, 1);
    m_shader->Use();
    m_shader->setIntUnf("u_material.albedo", m_albedo->getTextureUnit());
}

void Material::setAlbedoMap(Texture2D& textureObj)
{
    m_albedo = &textureObj;
    m_shader->Use();
    m_shader->setIntUnf("u_material.albedo", m_albedo->getTextureUnit());
}

void Material::setRoughnessMap(const FileSystem::Path& path)
{
    m_roughness = &g_Texture2DManager.getTexture2D(path, ETextureType::ROUGHNESS, 1, 1);
    m_shader->Use();
    m_shader->setIntUnf("u_material.roughness", m_roughness->getTextureUnit());
}

void Material::setRoughnessMap(Texture2D& textureObj)
{
    m_roughness = &textureObj;
    m_shader->Use();
    m_shader->setIntUnf("u_material.roughness", m_roughness->getTextureUnit());
}

void Material::setAoMap(const FileSystem::Path& path)
{
    m_ao = &g_Texture2DManager.getTexture2D(path, ETextureType::AO, 1, 1);
    m_shader->Use();
    m_shader->setIntUnf("u_material.ao", m_ao->getTextureUnit());
}

void Material::setAoMap(Texture2D& textureObj)
{
    m_ao = &textureObj;
    m_shader->Use();
    m_shader->setIntUnf("u_material.ao", m_ao->getTextureUnit());
}

void Material::setNormalMap(const FileSystem::Path& path)
{
    m_normal = &g_Texture2DManager.getTexture2D(path, ETextureType::NORMAL, 1, 1);
    m_shader->Use();
    m_shader->setIntUnf("u_material.normal", m_normal->getTextureUnit());
}

void Material::setNormalMap(Texture2D& textureObj)
{
    m_normal = &textureObj;
    m_shader->Use();
    m_shader->setIntUnf("u_material.normal", m_normal->getTextureUnit());
}

void Material::setSpecularMap(const FileSystem::Path& path)
{
    m_specular = &g_Texture2DManager.getTexture2D(path, ETextureType::SPECULAR, 1, 1);
    m_shader->Use();
    m_shader->setIntUnf("u_material.specular", m_specular->getTextureUnit());
}

void Material::setSpecularMap(Texture2D& textureObj)
{
    m_specular = &textureObj;
    m_shader->Use();
    m_shader->setIntUnf("u_material.specular", m_specular->getTextureUnit());
}

void Material::setShininess(float shininess)
{
    m_shininess = shininess;
}

std::shared_ptr<Shader> Material::getShader() const
{
    return m_shader;
}

void Material::setShader(std::shared_ptr<Shader> newShader)
{
    m_shader = newShader;
}

Texture2D const* Material::getAlbedoMap()
{
    return m_albedo;
}

Texture2D const* Material::getRoughnessMap()
{
    return m_roughness;
}

Texture2D const* Material::getAoMap()
{
    return m_ao;
}

Texture2D const* Material::getNormalMap()
{
    return m_normal;
}

Texture2D const* Material::getSpecularMap()
{
    return m_specular;
}

float Material::getShininess() const
{
    return m_shininess;
}

HNCRSP_NAMESPACE_END