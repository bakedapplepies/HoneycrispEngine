#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

enum ETextureType  // also acts as Texture Units
{
    NONE = -1,
    ALBEDO,
    ROUGHNESS,
    AO,
    NORMAL,
    SPECULAR
};

class Texture2D
{
private:
    int m_pixelWidth;
    int m_pixelHeight;
    
    GLuint m_textureID;
    ETextureType m_textureType;
    
public:
    Texture2D() = default;
    Texture2D(
        const FileSystem::Path& texturePath,
        ETextureType textureType
    );    
    ~Texture2D() = default;
    Texture2D(const Texture2D&);
    Texture2D& operator=(const Texture2D&);
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

public:
    const GLuint& getID() const;
    GLuint getTextureUnit() const;
    ETextureType getTextureType() const;
    int getWidth() const;
    int getHeight() const;

    void Bind() const;
    void Unbind() const;
    void Delete();
};

HNCRSP_NAMESPACE_END