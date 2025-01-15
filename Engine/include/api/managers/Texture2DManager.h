#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/Texture2D.h"
#include "api/graphics/TextureAtlas.h"


HNCRSP_NAMESPACE_START

// Manages texture retrieving
class Texture2DManager
{
private:
    std::unordered_map< std::string, std::unique_ptr<Texture2D> > m_cachedTexture2Ds;
    std::unordered_map< std::string, TextureAtlas > m_cachedTextureAtlases;
    int m_maxTextureUnitsPerStage;
    
public:
    const Texture2D* mainTextureMap;  // TODO: just use texture as regular path, no need for this
    const Texture2D* mainTextureSpecularMap;

public:
    Texture2DManager() = default;
    Texture2DManager(const Texture2DManager&) = delete;
    Texture2DManager& operator=(const Texture2DManager&) = delete;
    Texture2DManager(Texture2DManager&&) = delete;
    Texture2DManager& operator=(Texture2DManager&&) = delete;
    ~Texture2DManager() = default;

    HNCRSP_NODISCARD uint64_t StartUp();
    void ShutDown(uint64_t key);

    const Texture2D* GetTexture2D(
        const FileSystem::Path& path,
        ETextureType texture_type
    );
    TextureAtlas& GetAtlas(uint32_t width, uint32_t height);

    int GetMaxTextureUnits();
};

extern Texture2DManager g_Texture2DManager;

HNCRSP_NAMESPACE_END