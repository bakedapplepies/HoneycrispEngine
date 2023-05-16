#pragma once

#include "pch/pch.h"

struct TextureCoords
{
    glm::vec2 tl;  // top left
    glm::vec2 tr;  // top right
    glm::vec2 bl;  // bottom left
    glm::vec2 br;  // bottom right
};

class Texture
{
private:
    int m_pixelWidth, m_pixelHeight;
    int m_textureResolution = 16;
    
    GLuint m_textureID;
    std::unordered_map<int, TextureCoords> m_textureCoords;

public:
    Texture() = default;

    void LoadTexture(const char* texturePath);

    GLuint getID() const;

    void Bind(GLenum texture_unit) const;
    void Unbind() const;
    void Delete() const;

    void GenerateTextureCoords();
    TextureCoords GetTextureCoords(unsigned int textureType) const;
};

enum Textures
{
    GRASS_TOP,
    GRASS_SIDE,
    DIRT,
    END
};