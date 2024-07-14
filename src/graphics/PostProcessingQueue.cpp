#include "PostProcessingQueue.h"
#include "src/utils/TracyProfile.h"


HNCRSP_NAMESPACE_START

PostProcessingQueue::PostProcessingQueue(int width, int height, const VertexArray* screenQuadVAO) :
    m_framebufferOne(width, height),
    m_framebufferTwo(width, height)
{
    m_screenQuadVAO = screenQuadVAO;
}

void PostProcessingQueue::DrawSequence(const CallbackData* callbackData)
{
    ZoneScopedN("Post processing");
    m_screenQuadVAO->Bind();
    for (const Shader* shader : m_postprocessingShaders)
    {
        shader->Use();

        if (!m_drawToFramebufferOne)
        {
            m_framebufferTwo.Bind();
            m_framebufferOne.BindColorBuffer();
        }
        else
        {
            m_framebufferOne.Bind();
            m_framebufferTwo.BindColorBuffer();
        }
        glClear(GL_COLOR_BUFFER_BIT);
        GLCall(
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        m_drawToFramebufferOne = !m_drawToFramebufferOne;
    }
}

void PostProcessingQueue::BindInitialFramebuffer()
{
    if (m_drawToFramebufferOne)
        m_framebufferOne.Bind();
    else
        m_framebufferTwo.Bind();

    m_drawToFramebufferOne = !m_drawToFramebufferOne;
}

void PostProcessingQueue::AddPostprocessingPass(const Shader* postprocessing_shader)
{
    m_postprocessingShaders.push_back(postprocessing_shader);
}

GLuint PostProcessingQueue::GetCurrentFramebufferColorTexture() const
{
    if (m_drawToFramebufferOne)
        return m_framebufferTwo.GetColorTextureID();
    return m_framebufferOne.GetColorTextureID();
}

HNCRSP_NAMESPACE_END