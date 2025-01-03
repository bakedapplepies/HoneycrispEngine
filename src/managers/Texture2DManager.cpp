#include "Texture2DManager.h"
#include "src/utils/Logging.h"


HNCRSP_NAMESPACE_START

Texture2DManager g_Texture2DManager;

const Texture2D* Texture2DManager::GetTexture2D(
    const FileSystem::Path& path,
    ETextureType texture_type
) {
    if (m_cachedTexture2Ds.find(path.string()) == m_cachedTexture2Ds.end())
    {
        m_cachedTexture2Ds[path.string()] = std::make_unique<Texture2D>(path, texture_type);
    }
    return m_cachedTexture2Ds[path.string()].get();
}

TextureAtlas& Texture2DManager::GetAtlas(uint32_t width, uint32_t height)
{
    assert(width > 0 && height > 0 && "Invalid atlas width and/or height.");

    std::string pair = std::to_string(width) + std::to_string(height);
    if (m_cachedTextureAtlases.find(pair) == m_cachedTextureAtlases.end())
    {
        m_cachedTextureAtlases[pair] = TextureAtlas(width, height);
    }
    return m_cachedTextureAtlases[pair];
}

int Texture2DManager::GetMaxTextureUnits()
{
    return m_maxTextureUnitsPerStage;
}

void Texture2DManager::StartUp()
{
    stbi_set_flip_vertically_on_load(true);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureUnitsPerStage);

    mainTextureMap = GetTexture2D(
        FileSystem::Path("resources/textures/grass_textures.png"),
        ETextureType::ALBEDO
    );
    mainTextureSpecularMap = GetTexture2D(
        FileSystem::Path("resources/textures/grass_textures_specular_map.png"),
        ETextureType::SPECULAR
    );
}

void Texture2DManager::ShutDown()
{
    m_cachedTexture2Ds.clear();
}

HNCRSP_NAMESPACE_END