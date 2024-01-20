#include "Texture2DManager.h"


HNCRSP_NAMESPACE_START

Texture2D Texture2DManager::mainTextureMap;
Texture2D Texture2DManager::mainTextureSpecularMap;

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

void Texture2DManager::StartUp()
{
    stbi_set_flip_vertically_on_load(true);


    mainTextureMap = Texture2D(
        FileSystem::Path("resources/textures/grass_textures.png"),
        ETextureType::DIFFUSE,
        3, 1
    );
    mainTextureSpecularMap = Texture2D(
        FileSystem::Path("resources/textures/grass_textures_specular_map.png"),
        ETextureType::SPECULAR,
        3, 1
    );
}

void Texture2DManager::ShutDown()
{
    // m_texture2Ds.clear();
    Texture2D::DeleteAllTextures();
}

HNCRSP_NAMESPACE_END