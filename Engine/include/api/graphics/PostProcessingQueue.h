#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/Framebuffer.h"
#include "api/graphics/core/VertexArray.h"
#include "api/graphics/core/Shader.h"

HNCRSP_NAMESPACE_START

class PostProcessingQueue
{
private:
    Framebuffer m_framebufferOne;
    Framebuffer m_framebufferTwo;
    const VertexArray* m_screenQuadVAO;
    std::vector<const Shader*> m_postprocessingShaders;
    
    bool m_drawToFramebufferOne = true;

public:
    PostProcessingQueue(int width, int height, const VertexArray* screenQuadVAO);
    PostProcessingQueue(const PostProcessingQueue&) = delete;
    PostProcessingQueue& operator=(const PostProcessingQueue&) = delete;
    PostProcessingQueue(PostProcessingQueue&&) = delete;
    PostProcessingQueue& operator=(PostProcessingQueue&&) = delete;
    ~PostProcessingQueue() = default;

    // Ping-pong-ing between framebuffers is expensive so try to
    // make kind of an uber shader.
    void AddPostprocessingPass(const Shader* postprocessing_shader);

    // To get scene data.
    void BindInitialFramebuffer();

    // Ping pong between framebuffers to apply postprocessing effects.
    void DrawSequence();

    GLuint GetCurrentFramebufferColorTexture() const;
};

HNCRSP_NAMESPACE_END