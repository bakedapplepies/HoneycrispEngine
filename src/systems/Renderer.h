#pragma once

#include "src/pch/pch.h"
#include "src/ecs/System.h"
#include "src/components/Transform.h"

#include "src/managers/RenderContext.h"
#include "src/graphics/core/Shader.h"
#include "src/graphics/Cubemap.h"
#include "src/graphics/core/Framebuffer.h"
#include "src/graphics/core/DepthMap.h"
#include "src/graphics/DepthPassCamera.h"
#include "src/graphics/PostProcessingQueue.h"


HNCRSP_NAMESPACE_START

struct DrawElementsIndirectCommand;

class Renderer : public ECS::System
{
private:
    const CallbackData* m_callbackData;
    std::weak_ptr<Cubemap> m_weak_currentCubemap;
    std::vector<GLuint> m_shaderIDs_Order;
    std::unique_ptr<VertexArray> m_screenQuad;
    std::unique_ptr<PostProcessingQueue> m_postprocessing_queue;
    std::unique_ptr<DepthMap> m_depthMap;
    DepthPassCamera m_depthPassCamera;

public:
    Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;
    ~Renderer() = default;

    void Render() const;
    void SwitchCubemap(std::weak_ptr<Cubemap> weak_cubemap);

    void AddEntityUID(ECS::EntityUID entityUID) override;

private:
    void RenderDepthPass() const;
    void RenderScenePass() const;

    glm::mat4 GetModelMatrix(Transform& transform) const;
    void GenerateDrawCommands();
};

struct DrawElementsIndirectCommand
{
    GLuint count = 1;
    GLuint instanceCount = 1;
    GLuint firstIndex = 0;
    GLint baseVertex = 0;
    GLuint baseInstance = 0;
};

HNCRSP_NAMESPACE_END