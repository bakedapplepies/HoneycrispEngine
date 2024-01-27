#include "Texture2DManager.h"


HNCRSP_NAMESPACE_START

std::unique_ptr<Texture2D> Texture2DManager::mainTextureMap;
std::unique_ptr<Texture2D> Texture2DManager::mainTextureSpecularMap;

Texture2D& Texture2DManager::getTexture2D(
    const FileSystem::Path& path,
    ETextureType texture_type,
    unsigned int atlasVerticalRes,
    unsigned int atlasHorizontalRes
)
{
    m_texture2Ds[path.string()] = Texture2D(path, texture_type, atlasHorizontalRes, atlasVerticalRes);
    
    return m_texture2Ds[path.string()];
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
        ETextureType::DIFFUSE,
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
    // m_texture2Ds.clear();
    // HNCRSP_LOG_INFO("111111111");
    // Texture2D::DeleteAllTextures();
    // HNCRSP_LOG_INFO("222222222");
    mainTextureMap.reset();
    mainTextureSpecularMap.reset();
}

HNCRSP_NAMESPACE_END