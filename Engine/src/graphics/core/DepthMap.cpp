#include "api/graphics/core/DepthMap.h"

HNCRSP_NAMESPACE_START

DepthMap::DepthMap(int width, int height)
    : m_width(width), m_height(height)
{
    // Create Framebuffer ----------
    glGenFramebuffers(1, &m_framebufferID);
    Bind();

    // Create Color buffer ----------
    glGenTextures(1, &m_depthTexture_ID);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach depth buffer
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        m_depthTexture_ID,
        0
    );
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);  // No color data

    // Check validity ----------
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        HNCRSP_TERMINATE("Framebuffer incomplete.");
    }

    BindDepthTexture(DEPTH_BUFFER_TEXTURE_UNIT_INDEX);
}

DepthMap::~DepthMap()
{
    HNCRSP_CHECK_RENDER_CONTEXT();

    glDeleteFramebuffers(1, &m_framebufferID);
    glDeleteTextures(1, &m_depthTexture_ID);
}

static bool viewport = false;
void DepthMap::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

    // has to be like this so the scene is rendered to the depth buffer properly
    glViewport(
        0, 0, m_width, m_height
    );
}

void DepthMap::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthMap::BindDepthTexture(const uint16_t texture_unit) const
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture_ID);
}

GLuint DepthMap::GetTextureID() const
{
    return m_depthTexture_ID;
}

HNCRSP_NAMESPACE_END