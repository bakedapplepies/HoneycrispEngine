#pragma once

#include "src/pch/pch.h"
#include "src/renderer/Texture2D.h"
#include "src/types/Singleton.h"
#include "src/renderer/Texture2D.h"


HNCRSP_NAMESPACE_START

// Manages texture retrieving
namespace Texture2DManager
{
    extern std::unique_ptr<Texture2D> mainTextureMap;
    extern std::unique_ptr<Texture2D> mainTextureSpecularMap;

    Texture2D& getTexture2D(
        const FileSystem::Path& path,
        ETextureType texture_type,
        unsigned int atlasVerticalRes,
        unsigned int atlasHorizontalRes
    );

    int getMaxTextureUnits();

    void StartUp();
    void ShutDown();
}

HNCRSP_NAMESPACE_END