#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class Texture2DManager;
class Texture2D
{
private:
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
    const GLuint& GetID() const;
    GLuint GetTextureUnit() const;
    ETextureType GetTextureType() const;

    void Bind() const;
    void Unbind() const;
    void Delete();
};

HNCRSP_NAMESPACE_END