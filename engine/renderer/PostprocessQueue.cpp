#include "PostprocessQueue.h"

#include <tracy/Tracy.hpp>

#include "core/Assert.h"
#include "core/Constants.h"

HNCRSP_NAMESPACE_START

PostprocessQueue::PostprocessQueue(const Envy::EnvyInstance* envy_instance, int width, int height)
{
    // main texture for general postprocessing
    m_buffer = envy_instance->CreateTexture2DEmpty(Envy::TextureFormat::RGBA16F, width, height);

    // texture chain for bloom
    int bloomChainSize = 4;
    // Automatically find chain length with final mip width of around 64
    // int bloomChainSize = std::round(std::log2(static_cast<float>(width))) - 6;
    // E.g. 2560x1440 --- 5 mips --> 80x45

    m_bloomMips.reserve(bloomChainSize);
    for (int i = 0; i < bloomChainSize; i++)
    {
        width /= 2;
        height /= 2;
        
        m_bloomMips.push_back(envy_instance->CreateTexture2DEmpty(Envy::TextureFormat::RGBA16F, width, height));   
    }
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

const GLResource<Envy::Texture2D> PostprocessQueue::Bloom(const GLResource<Envy::Texture2D>& source,
                                                          const Envy::ShaderProgram* downsample_compute,
                                                          const Envy::ShaderProgram* upsample_compute) const
{
    _BloomDownsample(downsample_compute, source);
    glUseProgram(0);
    _BloomUpsample(upsample_compute, source);
    glUseProgram(0);

    return source;
}

void PostprocessQueue::_BloomDownsample(const Envy::ShaderProgram* downsample_compute, const GLResource<Envy::Texture2D>& source) const
{
    source->Bind(TEXTURE_UNIT_ALBEDO);
    downsample_compute->Bind();
    downsample_compute->UniformFloat2("u_inputRes", source->GetResolution());

    static const int LOCAL_X = 8;
    static const int LOCAL_Y = 8;

    for (uint32_t i = 0; i < m_bloomMips.size(); i++)
    {
        m_bloomMips[i]->MakeImageWriteAccess(IMAGE_UNIT_WRITE);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        glDispatchCompute(m_bloomMips[i]->GetWidth()/LOCAL_X, m_bloomMips[i]->GetHeight()/LOCAL_Y, 1);

        m_bloomMips[i]->Bind(TEXTURE_UNIT_ALBEDO);
        downsample_compute->UniformFloat2("u_inputRes", m_bloomMips[i]->GetResolution());
    }
}

void PostprocessQueue::_BloomUpsample(const Envy::ShaderProgram* upsample_compute, const GLResource<Envy::Texture2D>& source) const
{
    upsample_compute->Bind();
    upsample_compute->UniformFloat("u_filterRadius", 0.005f);

    static const int LOCAL_X = 8;
    static const int LOCAL_Y = 8;

    for (uint32_t i = m_bloomMips.size() - 1; i > 0; i--)
    {
        m_bloomMips[i]->Bind(TEXTURE_UNIT_ALBEDO);
        m_bloomMips[i-1]->MakeImageReadWriteAccess(IMAGE_UNIT_READ_WRITE);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        glDispatchCompute(m_bloomMips[i-1]->GetWidth()/LOCAL_X, m_bloomMips[i-1]->GetHeight()/LOCAL_Y, 1);
    }
    m_bloomMips[0]->Bind(TEXTURE_UNIT_ALBEDO);
    source->MakeImageReadWriteAccess(IMAGE_UNIT_READ_WRITE);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glDispatchCompute(source->GetWidth()/LOCAL_X, source->GetHeight()/LOCAL_Y, 1);
}

HNCRSP_NAMESPACE_END