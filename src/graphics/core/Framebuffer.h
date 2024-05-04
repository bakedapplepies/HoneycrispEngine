#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

class Framebuffer
{
private:
    GLuint m_framebufferID;
    GLuint m_textureColorBuffer;
    GLuint m_RBO;
    int m_width;
    int m_height;

public:
    Framebuffer(
        int width,
        int height
    );
    ~Framebuffer();

    void Bind() const;
    void Unbind() const;

    void BindColorBuffer() const;
};

HNCRSP_NAMESPACE_END