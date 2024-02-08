#include "Material.h"


HNCRSP_NAMESPACE_START

Material::Material(std::shared_ptr<Shader> shader)
{
    m_shader = shader;
}

Material::~Material()
{
    
}

void Material::setAlbedoMap(const FileSystem::Path& path)
{
    m_albedo = Texture2D(path, ETextureType::ALBEDO, 1, 1);
    m_shader->setIntUnf("u_material.albedo", m_albedo.getTextureUnit());
}

void Material::setRoughnessMap(const FileSystem::Path& path)
{
    m_roughness = Texture2D(path, ETextureType::ROUGHNESS, 1, 1);
    m_shader->setIntUnf("u_material.roughness", m_roughness.getTextureUnit());
}

void Material::setAoMap(const FileSystem::Path& path)
{
    m_ao = Texture2D(path, ETextureType::AO, 1, 1);
    m_shader->setIntUnf("u_material.ao", m_ao.getTextureUnit());
}

void Material::setNormalMap(const FileSystem::Path& path)
{
    m_normal = Texture2D(path, ETextureType::NORMAL, 1, 1);
    m_shader->setIntUnf("u_material.normal", m_normal.getTextureUnit());
}

void Material::setSpecularMap(const FileSystem::Path& path)
{
    m_specular = Texture2D(path, ETextureType::SPECULAR, 1, 1);
    m_shader->setIntUnf("u_material.specular", m_specular.getTextureUnit());
}

HNCRSP_NAMESPACE_END