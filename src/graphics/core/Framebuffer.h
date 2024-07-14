#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class Framebuffer
{
private:
    GLuint m_framebufferID;
    GLuint m_colorBufferTexture_ID;  // TODO: Maybe add another color buffer
    GLuint m_RBO_ID;
    int m_width;
    int m_height;

public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    void Bind() const;
    void Unbind() const;
    void BindColorBuffer() const;

    GLuint GetColorTextureID() const;
};

HNCRSP_NAMESPACE_END