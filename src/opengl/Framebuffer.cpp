#include "Framebuffer.h"


HNCRSP_NAMESPACE_START

Framebuffer::Framebuffer(
    int width,
    int height
) {
    // Create Framebuffer ----------
    GLCall(
        glGenFramebuffers(1, &m_framebufferID));
    Bind();

    // Create Color buffer ----------
    GLCall(
        glGenTextures(1, &m_textureColorBuffer));
    GLCall(
        glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer));
    GLCall(
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLCall(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLCall(  // attach color buffer
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_textureColorBuffer,
            0
        ));

    GLCall(
        glGenRenderbuffers(1, &m_RBO));
    GLCall(
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));
    GLCall(
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    GLCall(
        glBindRenderbuffer(GL_RENDERBUFFER, 0));

    // Check validity ----------
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        HNCRSP_TERMINATE("Framebuffer incomplete.");
    }
}

Framebuffer::~Framebuffer()
{

}

void Framebuffer::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));
}

void Framebuffer::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

HNCRSP_NAMESPACE_END