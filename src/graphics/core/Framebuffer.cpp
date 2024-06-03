#include "Framebuffer.h"
#include "src/managers/SceneManager.h"


HNCRSP_NAMESPACE_START

Framebuffer::Framebuffer(int width, int height)
    : m_width(width), m_height(height)
{
    // Create Framebuffer ----------
    GLCall(
        glGenFramebuffers(1, &m_framebufferID));
    Bind();

    // Create Color buffer ----------
    GLCall(
        glGenTextures(1, &m_colorBufferTexture_ID));
    GLCall(
        glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture_ID));
    GLCall(
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, width, height));
    GLCall(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    // Attach color buffer
    GLCall(
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_colorBufferTexture_ID,
            0
        ));

    // Creating Depth - Stencil buffer
    GLCall(
        glGenRenderbuffers(1, &m_RBO_ID));
    GLCall(
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID));
    GLCall(
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    GLCall(
        glBindRenderbuffer(GL_RENDERBUFFER, 0));
    GLCall(
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID));

    // Check validity ----------
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        HNCRSP_TERMINATE("Framebuffer incomplete.");
    }
}

Framebuffer::~Framebuffer()
{
    HNCRSP_CHECK_RENDER_CONTEXT();

    GLCall(
        glDeleteFramebuffers(1, &m_framebufferID));
    GLCall(
        glDeleteTextures(1, &m_colorBufferTexture_ID));
    GLCall(
        glDeleteRenderbuffers(1, &m_RBO_ID));
}

static bool viewport = false;
void Framebuffer::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));

    GLCall(  // has to be like this so the scene is rendered to the color buffer properly
        glViewport(
            0, 0, m_width, m_height
        ));
}

void Framebuffer::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::BindColorBuffer() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + COLOR_BUFFER_TEXTURE_UNIT_INDEX));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture_ID));
}

HNCRSP_NAMESPACE_END