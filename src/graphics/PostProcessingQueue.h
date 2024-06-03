#pragma once

#include "src/pch/pch.h"
#include "src/managers/RenderContext.h"
#include "src/graphics/core/Framebuffer.h"
#include "src/graphics/core/VertexArray.h"
#include "src/graphics/core/Shader.h"


HNCRSP_NAMESPACE_START

class PostProcessingQueue
{
private:
    std::unique_ptr<Framebuffer> m_framebuffer_one;
    std::unique_ptr<Framebuffer> m_framebuffer_two;
    const VertexArray* m_screenQuadVAO;
    std::vector<const Shader*> m_postprocessing_shaders;
    
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
    void DrawSequence(const CallbackData* callbackData);
};

HNCRSP_NAMESPACE_END