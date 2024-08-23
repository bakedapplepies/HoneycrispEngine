#include "DepthMap.h"


HNCRSP_NAMESPACE_START

DepthMap::DepthMap(int width, int height)
    : m_width(width), m_height(height)
{
    // Create Framebuffer ----------
    GLCall(
        glGenFramebuffers(1, &m_framebufferID));
    Bind();

    // Create Color buffer ----------
    GLCall(
        glGenTextures(1, &m_depthTexture_ID));
    GLCall(
        glBindTexture(GL_TEXTURE_2D, m_depthTexture_ID));
    GLCall(
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
    GLCall(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    // Attach depth buffer
    GLCall(
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            m_depthTexture_ID,
            0
        ));
    GLCall(
        glDrawBuffer(GL_NONE));
    GLCall(
        glReadBuffer(GL_NONE));

    // Check validity ----------
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        HNCRSP_TERMINATE("Framebuffer incomplete.");
    }
}

DepthMap::~DepthMap()
{
    HNCRSP_CHECK_RENDER_CONTEXT();

    GLCall(
        glDeleteFramebuffers(1, &m_framebufferID));
    GLCall(
        glDeleteTextures(1, &m_depthTexture_ID));
}

static bool viewport = false;
void DepthMap::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));

    GLCall(  // has to be like this so the scene is rendered to the color buffer properly
        glViewport(
            0, 0, m_width, m_height
        ));
}

void DepthMap::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void DepthMap::BindDepthBuffer(const uint16_t texture_unit) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_depthTexture_ID));
}

HNCRSP_NAMESPACE_END