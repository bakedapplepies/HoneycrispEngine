#include "Texture2DManager.h"
#include "src/utils/Logging.h"


HNCRSP_NAMESPACE_START

Texture2DManager g_Texture2DManager;

Texture2D& Texture2DManager::getTexture2D(
    const FileSystem::Path& path,
    ETextureType texture_type,
    unsigned int atlasVerticalRes,
    unsigned int atlasHorizontalRes
) {
    m_cachedTexture2Ds[path.string()] = Texture2D(path, texture_type, atlasHorizontalRes, atlasVerticalRes);
    
    return m_cachedTexture2Ds[path.string()];
}

int Texture2DManager::getMaxTextureUnits()
{
    return m_maxTextureUnitsPerStage;
}

void Texture2DManager::StartUp()
{
    stbi_set_flip_vertically_on_load(true);

    GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureUnitsPerStage));

    mainTextureMap = std::make_unique<Texture2D>(
        FileSystem::Path("resources/textures/grass_textures.png"),
        ETextureType::ALBEDO,
        3, 1
    );
    mainTextureSpecularMap = std::make_unique<Texture2D>(
        FileSystem::Path("resources/textures/grass_textures_specular_map.png"),
        ETextureType::SPECULAR,
        3, 1
    );
}

void Texture2DManager::ShutDown()
{
    mainTextureMap.reset();
    mainTextureSpecularMap.reset();
    for (auto iter : m_cachedTexture2Ds)
    {
        iter.second.Delete();
    }
}

HNCRSP_NAMESPACE_END