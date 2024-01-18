#pragma once

#include "src/pch/pch.h"
#include "../utils/utils.h"

enum ETextureType
{
    DIFFUSE,
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
    int m_textureWidth;
    int m_textureHeight;
    
    GLuint m_textureID;
    std::vector< std::vector<TextureCoords> > m_textureCoords;
    std::string path;
    ETextureType m_textureType;
    static GLuint sm_textureUnitCounter;
    static std::unordered_map<std::string, TextureInfo> sm_initiatedTextures;
    static std::unordered_map<GLuint, GLint> sm_textureUnits;  // TODO: Convert GLint to vector to reuse texture units
    static std::unordered_map<GLuint, unsigned int> sm_textureIDCount;

public:
    Texture2D() = default;
    ~Texture2D();
    Texture2D(
        const Honeycrisp::FileSystem::Path& texturePath,
        uint32_t textureResolutionWidth,
        uint32_t textureResolutionHeight,
        ETextureType textureType
    );    
    Texture2D(const Texture2D&);
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(const Texture2D&);
    Texture2D& operator=(Texture2D&& other) noexcept;

    void GenerateTextureCoords();

public:
    GLuint getID() const;
    GLuint getTextureUnit() const;
    ETextureType getTextureType() const;

    void Bind() const;
    void Unbind() const;
    void Delete();

    static void LoadTextures();
    static void DeleteAllTextures();
    TextureCoords& GetTextureCoords(uint32_t x, uint32_t y);
};

namespace Textures
{
    extern Texture2D mainTextureMap;
    extern Texture2D mainTextureSpecularMap;
}