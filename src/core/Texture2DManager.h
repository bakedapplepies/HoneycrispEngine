#pragma once

#include "src/pch/pch.h"
#include "src/renderer/Texture2D.h"
#include "src/types/Singleton.h"
#include "src/renderer/Texture2D.h"


HNCRSP_NAMESPACE_START

// Manages texture retrieving
namespace Texture2DManager
{
    namespace
    {
        // TODO: see if this should be used (std::unordered_map)
        static std::unordered_map<std::string, Texture2D> m_texture2Ds;

        // void Create
    }

    extern Texture2D mainTextureMap;
    extern Texture2D mainTextureSpecularMap;

    Texture2D& getTexture2D(
        const FileSystem::Path& path,
        ETextureType texture_type,
        unsigned int atlasVerticalRes,
        unsigned int atlasHorizontalRes
    );

    void StartUp();
    void ShutDown();
}

HNCRSP_NAMESPACE_END