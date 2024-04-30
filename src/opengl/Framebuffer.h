#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

class Framebuffer
{
private:
    GLuint m_framebufferID;
    GLuint m_textureColorBuffer;
    GLuint m_RBO;

public:
    Framebuffer(
        int width,
        int height
    );
    ~Framebuffer();

    void Bind() const;
    void Unbind() const;
};

HNCRSP_NAMESPACE_END