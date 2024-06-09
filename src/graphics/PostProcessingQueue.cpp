#include "PostProcessingQueue.h"


HNCRSP_NAMESPACE_START

PostProcessingQueue::PostProcessingQueue(int width, int height, const VertexArray* screenQuadVAO)
{
    m_framebufferOne = std::make_unique<Framebuffer>(width, height);
    m_framebufferTwo = std::make_unique<Framebuffer>(width, height);
    m_screenQuadVAO = screenQuadVAO;
}

void PostProcessingQueue::DrawSequence(const CallbackData* callbackData)
{
    m_screenQuadVAO->Bind();
    for (const Shader* shader : m_postprocessingShaders)
    {
        shader->Use();

        if (!m_drawToFramebufferOne)
        {
            m_framebufferTwo->Bind();
            m_framebufferOne->BindColorBuffer();
        }
        else
        {
            m_framebufferOne->Bind();
            m_framebufferTwo->BindColorBuffer();
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        GLCall(
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        m_drawToFramebufferOne = !m_drawToFramebufferOne;
    }

    if (m_drawToFramebufferOne)
        m_framebufferOne->Unbind();
    else
        m_framebufferTwo->Unbind();

    glClear(GL_COLOR_BUFFER_BIT);
    GLCall(  // resetting viewport to offset ImGui settings
        glViewport(
            callbackData->windowWidth * callbackData->settingsWidthPercentage,
            0,
            callbackData->windowWidth * (1.0f - callbackData->settingsWidthPercentage),
            callbackData->windowHeight
        ));
    GLCall(
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void PostProcessingQueue::BindInitialFramebuffer()
{
    if (m_drawToFramebufferOne)
        m_framebufferOne->Bind();
    else
        m_framebufferTwo->Bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_drawToFramebufferOne = !m_drawToFramebufferOne;
}

void PostProcessingQueue::AddPostprocessingPass(const Shader* postprocessing_shader)
{
    m_postprocessingShaders.push_back(postprocessing_shader);
}

HNCRSP_NAMESPACE_END