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
    int m_textureResolutionWidth;
    int m_textureResolutionHeight;
    
    
    GLuint m_textureID;
    std::vector<std::vector<TextureCoords>> m_textureCoords;
    static GLint m_maxTextureUnits;
    static GLuint sm_textureUnitCounter;
    static std::unordered_map<GLuint, GLint> sm_textureUnits;
    static std::vector<Texture*> s_textureRefs;

public:
    Texture();
    ~Texture();

private:
    Texture(const char* texturePath, uint32_t textureResolutionWidth, uint32_t textureResolutionHeight);
    
    // Move constructor/assigntment are private so it's safe
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