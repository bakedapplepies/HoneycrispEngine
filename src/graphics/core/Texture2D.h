#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class Texture2DManager;
class Texture2D
{
private:
    GLuint m_textureID;
    int32_t m_channels;
    ETextureType m_textureType;
    
public:
    Texture2D() = default;
    Texture2D(const FileSystem::Path& texturePath, ETextureType textureType);    
    ~Texture2D();
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&& other) noexcept = delete;
    Texture2D& operator=(Texture2D&& other) noexcept = delete;

public:
    const GLuint& GetID() const;
    GLuint GetTextureUnit() const;
    int32_t GetChannels() const;
    ETextureType GetTextureType() const;

    inline void Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + GetTextureUnit()));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
    }

    inline void Unbind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + GetTextureUnit()));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void Delete();
};

HNCRSP_NAMESPACE_END