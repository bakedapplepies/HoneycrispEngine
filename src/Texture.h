#pragma once

#include "pch/pch.h"

class Texture
{
private:
    GLuint m_textureID;

public:
    void CreateTexture(const std::string& texturePath);

    GLuint getID() const;

    void Bind() const;
    void Unbind() const;
    void Delete() const;
};