#include "Material.h"


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
    m_albedo = std::make_shared<Texture2D>(path, ETextureType::ALBEDO, 1, 1);
    m_shader->setIntUnf("u_material.albedo", m_albedo->getTextureUnit());
}

void Material::setRoughnessMap(const FileSystem::Path& path)
{
    m_roughness = std::make_shared<Texture2D>(path, ETextureType::ROUGHNESS, 1, 1);
    m_shader->setIntUnf("u_material.roughness", m_roughness->getTextureUnit());
}

void Material::setAoMap(const FileSystem::Path& path)
{
    m_ao = std::make_shared<Texture2D>(path, ETextureType::AO, 1, 1);
    m_shader->setIntUnf("u_material.ao", m_ao->getTextureUnit());
}

void Material::setNormalMap(const FileSystem::Path& path)
{
    m_normal = std::make_shared<Texture2D>(path, ETextureType::NORMAL, 1, 1);
    m_shader->setIntUnf("u_material.normal", m_normal->getTextureUnit());
}

void Material::setSpecularMap(const FileSystem::Path& path)
{
    m_specular = std::make_shared<Texture2D>(path, ETextureType::SPECULAR, 1, 1);
    m_shader->setIntUnf("u_material.specular", m_specular->getTextureUnit());
}

void Material::setShininess(float shininess)
{
    m_shininess = shininess;
}

std::weak_ptr<Texture2D> Material::getAlbedoMap() const
{
    return m_albedo;
}

std::weak_ptr<Texture2D> Material::getRoughnessMap() const
{
    return std::weak_ptr<Texture2D>(m_roughness);
}

std::weak_ptr<Texture2D> Material::getAoMap() const
{
    return std::weak_ptr<Texture2D>(m_ao);
}

std::weak_ptr<Texture2D> Material::getNormalMap() const
{
    return std::weak_ptr<Texture2D>(m_normal);
}

std::weak_ptr<Texture2D> Material::getSpecularMap() const
{
    return std::weak_ptr<Texture2D>(m_specular);
}

float Material::getShininess() const
{
    return m_shininess;
}

HNCRSP_NAMESPACE_END