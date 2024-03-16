#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Texture2D.h"
#include "src/types/Singleton.h"
#include "src/opengl/Texture2D.h"


HNCRSP_NAMESPACE_START

// Manages texture retrieving
class Texture2DManager
{
private:
    // TODO: is caching with paths the best?
    std::unordered_map<std::string, Texture2D> m_cachedTexture2Ds;
    int m_maxTextureUnitsPerStage;
    
public:
    std::unique_ptr<Texture2D> mainTextureMap;  // TODO: just use texture as regular path, no need for this
    std::unique_ptr<Texture2D> mainTextureSpecularMap;

public:
    Texture2DManager() = default;
    Texture2DManager(const Texture2DManager&) = delete;
    Texture2DManager& operator=(const Texture2DManager&) = delete;
    Texture2DManager(Texture2DManager&&) = delete;
    Texture2DManager& operator=(Texture2DManager&&) = delete;
    ~Texture2DManager() = default;

    void StartUp();
    void ShutDown();

    Texture2D& getTexture2D(
        const FileSystem::Path& path,
        ETextureType texture_type,
        unsigned int atlasVerticalRes,
        unsigned int atlasHorizontalRes
    );

    int getMaxTextureUnits();
};

extern Texture2DManager g_Texture2DManager;

HNCRSP_NAMESPACE_END