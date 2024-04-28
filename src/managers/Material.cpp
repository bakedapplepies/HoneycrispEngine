#include "Material.h"
#include "src/managers/Texture2DManager.h"


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
    m_albedo = g_Texture2DManager.GetTexture2D(path, ETextureType::ALBEDO).get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.albedo", m_albedo->getTextureUnit());
}

void Material::setAlbedoMap(std::shared_ptr<Texture2D> textureObj)
{
    m_albedo = textureObj.get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.albedo", textureObj->getTextureUnit());
}

void Material::setRoughnessMap(const FileSystem::Path& path)
{
    m_roughness = g_Texture2DManager.GetTexture2D(path, ETextureType::ROUGHNESS).get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.roughness", m_roughness->getTextureUnit());
}

void Material::setRoughnessMap(std::shared_ptr<Texture2D> textureObj)
{
    m_roughness = textureObj.get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.roughness", textureObj->getTextureUnit());
}

void Material::setAoMap(const FileSystem::Path& path)
{
    m_ao = g_Texture2DManager.GetTexture2D(path, ETextureType::AO).get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.ao", m_ao->getTextureUnit());
}

void Material::setAoMap(std::shared_ptr<Texture2D> textureObj)
{
    m_ao = textureObj.get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.ao", textureObj->getTextureUnit());
}

void Material::setNormalMap(const FileSystem::Path& path)
{
    m_normal = g_Texture2DManager.GetTexture2D(path, ETextureType::NORMAL).get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.normal", m_normal->getTextureUnit());
}

void Material::setNormalMap(std::shared_ptr<Texture2D> textureObj)
{
    m_normal = textureObj.get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.normal", textureObj->getTextureUnit());
}

void Material::setSpecularMap(const FileSystem::Path& path)
{
    m_specular = g_Texture2DManager.GetTexture2D(path, ETextureType::SPECULAR).get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.specular", m_specular->getTextureUnit());
}

void Material::setSpecularMap(std::shared_ptr<Texture2D> textureObj)
{
    m_specular = textureObj.get();
    m_shader->Use();
    m_shader->setIntUnf("u_material.specular", textureObj->getTextureUnit());
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

const Texture2D* Material::getAlbedoMap()
{
    return m_albedo;
}

const Texture2D* Material::getRoughnessMap()
{
    return m_roughness;
}

const Texture2D* Material::getAoMap()
{
    return m_ao;
}

const Texture2D* Material::getNormalMap()
{
    return m_normal;
}

const Texture2D* Material::getSpecularMap()
{
    return m_specular;
}

float Material::getShininess() const
{
    return m_shininess;
}

HNCRSP_NAMESPACE_END