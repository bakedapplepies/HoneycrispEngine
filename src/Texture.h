#pragma once

#include "pch/pch.h"

class Texture
{
private:
    GLuint m_textureID;

public:
    void CreateTexture(const char* texturePath);

    GLuint getID() const;

    void Bind(GLenum texture_unit) const;
    void Unbind() const;
    void Delete() const;
};