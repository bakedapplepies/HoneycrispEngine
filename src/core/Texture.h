#pragma once

#include "../pch/pch.h"

struct TextureCoords
{
    glm::vec2 tl;  // top left
    glm::vec2 tr;  // top right
    glm::vec2 bl;  // bottom left
    glm::vec2 br;  // bottom right
};

// static std::vector<Texture> a[32];

class Texture
{
private:
    int m_pixelWidth, m_pixelHeight;
    int m_textureWidth;
    int m_textureHeight;
    
    GLuint m_textureID;
    std::vector< std::vector<TextureCoords> > m_textureCoords;
    std::string path;
    static GLuint sm_textureUnitCounter;
    static std::unordered_map<std::string, Texture*> sm_initiatedTextures;
    static std::unordered_map<GLuint, GLint> sm_textureUnits;  // TODO: Convert GLint to vector to reuse texture units
    static std::unordered_map<GLuint, unsigned int> sm_textureIDCount;
    static std::vector< std::shared_ptr<Texture> > sm_textureRefs;

public:
    Texture() = default;
    ~Texture();
    Texture(const std::string& texturePath, uint32_t textureResolutionWidth, uint32_t textureResolutionHeight, const std::source_location& location = std::source_location::current());    
    Texture(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other) noexcept;

    void GenerateTextureCoords();

public:
    GLuint getID() const;
    GLint getTextureUnit() const;

    void Bind() const;
    void Unbind() const;
    void Delete();

    static void LoadTextures();
    static void DeleteAllTextures();
    TextureCoords& GetTextureCoords(uint32_t x, uint32_t y);
};

namespace Textures {
    extern Texture mainTextureMap;
    extern Texture mainTextureSpecularMap;
}