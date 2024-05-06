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

class Texture2DManager;
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
    ~Texture2D();
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&& other) noexcept = delete;
    Texture2D& operator=(Texture2D&& other) noexcept = delete;

public:
    const GLuint& getID() const;
    GLuint getTextureUnit() const;
    ETextureType getTextureType() const;
    int getWidth() const;
    int getHeight() const;

    void Bind() const;
    void Unbind() const;
};

HNCRSP_NAMESPACE_END