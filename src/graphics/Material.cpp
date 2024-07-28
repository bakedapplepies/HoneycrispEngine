#include "Material.h"
#include "src/managers/Texture2DManager.h"


HNCRSP_NAMESPACE_START

Material::Material(const Shader* shader) :
    m_shader(nullptr),
    m_albedo(nullptr),
    m_roughness(nullptr),
    m_ao(nullptr),
    m_normal(nullptr),
    m_specular(nullptr),
    m_shininess(64.0f)
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
    m_albedo = other.m_albedo;
    m_roughness = other.m_roughness;
    m_ao = other.m_ao;
    m_normal = other.m_normal;
    m_specular = other.m_specular;
    m_shininess = other.m_shininess;

    m_albedo = nullptr;
    m_roughness = nullptr;
    m_ao = nullptr;
    m_normal = nullptr;
    m_specular = nullptr;
    other.m_shininess = 0.0f;
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

    m_albedo = nullptr;
    m_roughness = nullptr;
    m_ao = nullptr;
    m_normal = nullptr;
    m_specular = nullptr;
    other.m_shininess = 0.0f;

    return *this;
}

void Material::SetAlbedoMap(const FileSystem::Path& path)
{
    m_albedo = g_Texture2DManager.GetTexture2D(path, ETextureType::ALBEDO);
}

void Material::SetAlbedoMap(const Texture2D* textureObj)
{
    m_albedo = textureObj;
}

void Material::SetRoughnessMap(const FileSystem::Path& path)
{
    m_roughness = g_Texture2DManager.GetTexture2D(path, ETextureType::ROUGHNESS);
}

void Material::SetRoughnessMap(const Texture2D* textureObj)
{
    m_roughness = textureObj;
}

void Material::SetAoMap(const FileSystem::Path& path)
{
    m_ao = g_Texture2DManager.GetTexture2D(path, ETextureType::AO);
}

void Material::SetAoMap(const Texture2D* textureObj)
{
    m_ao = textureObj;
}

void Material::SetNormalMap(const FileSystem::Path& path)
{
    m_normal = g_Texture2DManager.GetTexture2D(path, ETextureType::NORMAL);
}

void Material::SetNormalMap(const Texture2D* textureObj)
{
    m_normal = textureObj;
}

void Material::SetSpecularMap(const FileSystem::Path& path)
{
    m_specular = g_Texture2DManager.GetTexture2D(path, ETextureType::SPECULAR);
}

void Material::SetSpecularMap(const Texture2D* textureObj)
{
    m_specular = textureObj;
}

void Material::SetShininess(float shininess)
{
    m_shininess = shininess;
}

const Shader* Material::getShader() const
{
    return m_shader;
}

void Material::SetShader(const Shader* newShader)
{
    m_shader = newShader;
    _UpdateTextureUniforms();
}

const Texture2D* Material::GetAlbedoMap()
{
    return m_albedo;
}

const Texture2D* Material::GetRoughnessMap()
{
    return m_roughness;
}

const Texture2D* Material::GetAoMap()
{
    return m_ao;
}

const Texture2D* Material::GetNormalMap()
{
    return m_normal;
}

const Texture2D* Material::GetSpecularMap()
{
    return m_specular;
}

float Material::GetShininess() const
{
    return m_shininess;
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