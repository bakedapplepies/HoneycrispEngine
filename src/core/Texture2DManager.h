#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Texture2D.h"
#include "src/opengl/TextureAtlas.h"


HNCRSP_NAMESPACE_START

// Manages texture retrieving
class Texture2DManager
{
private:
    std::unordered_map< std::string, std::shared_ptr<Texture2D> > m_cachedTexture2Ds;
    std::unordered_map< std::string, TextureAtlas > m_cachedTextureAtlases;
    int m_maxTextureUnitsPerStage;
    
public:
    std::shared_ptr<Texture2D> mainTextureMap;  // TODO: just use texture as regular path, no need for this
    std::shared_ptr<Texture2D> mainTextureSpecularMap;

public:
    Texture2DManager() = default;
    Texture2DManager(const Texture2DManager&) = delete;
    Texture2DManager& operator=(const Texture2DManager&) = delete;
    Texture2DManager(Texture2DManager&&) = delete;
    Texture2DManager& operator=(Texture2DManager&&) = delete;
    ~Texture2DManager() = default;

    void StartUp();
    void ShutDown();

    std::shared_ptr<Texture2D> GetTexture2D(
        const FileSystem::Path& path,
        ETextureType texture_type
    );
    TextureAtlas& GetAtlas(uint32_t width, uint32_t height);

    int GetMaxTextureUnits();
};

extern Texture2DManager g_Texture2DManager;

HNCRSP_NAMESPACE_END