#include "Material.h"
#include "src/managers/Texture2DManager.h"


HNCRSP_NAMESPACE_START

Material::Material(const Shader* shader)
{
    SetShader(shader);
    shader->SetFloatUnf("u_material.shininess", m_shininess);
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
    m_isOpaque = other.m_isOpaque;
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
    m_isOpaque = other.m_isOpaque;

    return *this;
}

Material::Material(Material&& other)
{
    m_shader = std::move(other.m_shader);
    m_albedo = other.m_albedo;
    m_roughness = other.m_roughness;
    m_ao = other.m_ao;
    m_normal = other.m_normal;
    m_specular = other.m_specular;
    m_shininess = other.m_shininess;
    m_isOpaque = other.m_isOpaque;

    other.m_albedo = nullptr;
    other.m_roughness = nullptr;
    other.m_ao = nullptr;
    other.m_normal = nullptr;
    other.m_specular = nullptr;
    other.m_shininess = 0.0f;
    other.m_isOpaque = true;
}

Material& Material::operator=(Material&& other) noexcept
{
    m_shader = std::move(other.m_shader);
    m_albedo = other.m_albedo;
    m_roughness = other.m_roughness;
    m_ao = other.m_ao;
    m_normal = other.m_normal;
    m_specular = other.m_specular;
    m_shininess = other.m_shininess;
    m_isOpaque = other.m_isOpaque;

    other.m_albedo = nullptr;
    other.m_roughness = nullptr;
    other.m_ao = nullptr;
    other.m_normal = nullptr;
    other.m_specular = nullptr;
    other.m_shininess = 0.0f;
    other.m_isOpaque = true;

    return *this;
}

void Material::SetShader(const Shader* new_shader)
{
    m_shader = new_shader;
    _UpdateTextureUniforms();
}

void Material::SetAlbedoMap(const FileSystem::Path& path)
{
    m_albedo = g_Texture2DManager.GetTexture2D(path, ETextureType::ALBEDO);
    if (m_albedo->GetChannels() == 4)
        m_isOpaque = false;
}

void Material::SetAlbedoMap(const Texture2D* texture_obj)
{
    m_albedo = texture_obj;
    if (m_albedo->GetChannels() == 4)
        m_isOpaque = false;
}

void Material::SetRoughnessMap(const FileSystem::Path& path)
{
    m_roughness = g_Texture2DManager.GetTexture2D(path, ETextureType::ROUGHNESS);
}

void Material::SetRoughnessMap(const Texture2D* texture_obj)
{
    m_roughness = texture_obj;
}

void Material::SetAoMap(const FileSystem::Path& path)
{
    m_ao = g_Texture2DManager.GetTexture2D(path, ETextureType::AO);
}

void Material::SetAoMap(const Texture2D* texture_obj)
{
    m_ao = texture_obj;
}

void Material::SetNormalMap(const FileSystem::Path& path)
{
    m_normal = g_Texture2DManager.GetTexture2D(path, ETextureType::NORMAL);
}

void Material::SetNormalMap(const Texture2D* texture_obj)
{
    m_normal = texture_obj;
}

void Material::SetSpecularMap(const FileSystem::Path& path)
{
    m_specular = g_Texture2DManager.GetTexture2D(path, ETextureType::SPECULAR);
}

void Material::SetSpecularMap(const Texture2D* texture_obj)
{
    m_specular = texture_obj;
}

void Material::SetShininess(float shininess)
{
    m_shininess = shininess;
}

void Material::SetIsOpaque(bool is_opaque)
{
    m_isOpaque = is_opaque;
}

void Material::_UpdateTextureUniforms() const
{
    m_shader->SetIntUnf("u_material.albedo", static_cast<int>(ETextureType::ALBEDO));
    m_shader->SetIntUnf("u_material.roughness", static_cast<int>(ETextureType::ROUGHNESS));
    m_shader->SetIntUnf("u_material.ao", static_cast<int>(ETextureType::AO));
    m_shader->SetIntUnf("u_material.normal", static_cast<int>(ETextureType::NORMAL));
    m_shader->SetIntUnf("u_material.specular", static_cast<int>(ETextureType::SPECULAR));
}

HNCRSP_NAMESPACE_END