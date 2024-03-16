#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

enum ETextureType
{
    NONE = -1,
    ALBEDO,
    ROUGHNESS,
    AO,
    NORMAL,
    SPECULAR
};

struct TextureCoords
{
    glm::vec2 tl;  // top left
    glm::vec2 tr;  // top right
    glm::vec2 bl;  // bottom left
    glm::vec2 br;  // bottom right
};

// static std::vector<Texture> a[32];

struct TextureInfo
{
    GLuint id = 0;
    std::vector< std::vector<TextureCoords> > textureCoords;
};

class Texture2D
{
private:
    int m_pixelWidth, m_pixelHeight;
    uint32_t m_atlasWidth;
    uint32_t m_atlasHeight;
    
    GLuint m_textureID;
    ETextureType m_textureType;
    std::string path;
    std::vector< std::vector<TextureCoords> > m_textureCoords;
    
    static GLuint sm_textureUnitCounter;
    static std::unordered_map<std::string, TextureInfo> sm_initiatedTextures;
    static std::unordered_map<GLuint, GLint> sm_textureUnits;  // TODO: Reuse texture units somehow

public:
    Texture2D() = default;
    ~Texture2D();
    Texture2D(
        const FileSystem::Path& texturePath,
        ETextureType textureType,
        uint32_t atlasWidth,
        uint32_t atlasHeight
    );    
    Texture2D(const Texture2D&);
    Texture2D& operator=(const Texture2D&);
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    void GenerateTextureCoords();

public:
    const GLuint& getID() const;
    GLuint getTextureUnit() const;
    ETextureType getTextureType() const;
    int getWidth() const;
    int getHeight() const;

    void Bind() const;
    void Unbind() const;
    void Delete();

    TextureCoords& GetTextureCoords(uint32_t x, uint32_t y);
};

HNCRSP_NAMESPACE_END