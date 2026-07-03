#include "PostprocessQueue.h"

#include <tracy/Tracy.hpp>

#include "core/Assert.h"
#include "core/Constants.h"

HNCRSP_NAMESPACE_START

PostprocessQueue::PostprocessQueue(const Envy::EnvyInstance* envy_instance, int width, int height)
{
    m_buffer = envy_instance->CreateTexture2DEmpty(Envy::TextureFormat::RGBA8, width, height);
}

PostprocessQueue::~PostprocessQueue()
{
    m_computeShaders.clear();
    m_buffer = GLResource<Envy::Texture2D>::empty;
}

void PostprocessQueue::AddCompute(const Envy::ShaderProgram* compute_shader)
{
    HNCRSP_ASSERT(compute_shader->GetShaderType() == Envy::ShaderType::COMPUTE,
                  "Shader given to PostprocessQueue is not a compute shader.");

    m_computeShaders.push_back(compute_shader);
}

const GLResource<Envy::Texture2D> PostprocessQueue::Process(const GLResource<Envy::Texture2D>& input,
                                                            int group_x, int group_y, int group_z) const
{
    uint32_t i;
    for (i = 0; i < m_computeShaders.size(); i++)
    {
        m_computeShaders[i]->Bind();
        if (i % 2 == 0)
        {
            input->MakeImageReadAccess(IMAGE_UNIT_READ);
            m_buffer->MakeImageWriteAccess(IMAGE_UNIT_WRITE);
        }
        else
        {
            m_buffer->MakeImageReadAccess(IMAGE_UNIT_READ);
            input->MakeImageWriteAccess(IMAGE_UNIT_WRITE);
        }
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        glDispatchCompute(group_x, group_y, group_z);
    }
    
    glUseProgram(0);
    if (i % 2 == 1)
        return m_buffer;
    else
        return input;
}

HNCRSP_NAMESPACE_END