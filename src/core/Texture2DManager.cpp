#include "Texture2DManager.h"
#include "src/utils/Logging.h"


HNCRSP_NAMESPACE_START

namespace Texture2DManager
{
    namespace
    {
        // TODO: see if this should be used (std::unordered_map)
        std::unordered_map<std::string, Texture2D> m_texture2Ds;
        int m_maxTextureUnitsPerStage;
    }

    std::unique_ptr<Texture2D> mainTextureMap;
    std::unique_ptr<Texture2D> mainTextureSpecularMap;

    Texture2D& getTexture2D(
        const FileSystem::Path& path,
        ETextureType texture_type,
        unsigned int atlasVerticalRes,
        unsigned int atlasHorizontalRes
    )
    {
        m_texture2Ds[path.string()] = Texture2D(path, texture_type, atlasHorizontalRes, atlasVerticalRes);
        
        return m_texture2Ds[path.string()];
    }

    int getMaxTextureUnits()
    {
        return m_maxTextureUnitsPerStage;
    }

    void StartUp()
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

    void ShutDown()
    {
        mainTextureMap.reset();
        mainTextureSpecularMap.reset();
    }

}

HNCRSP_NAMESPACE_END