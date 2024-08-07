#pragma once

#include "src/pch/hncrsp_pch.h"
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

struct DrawElementsIndirectCommand;  // end of file

class Renderer : public ECS::System
{
private:
    const CallbackData* m_callbackData;
    const Cubemap* m_currentCubemap = nullptr;
    std::vector< std::vector<GLuint> > m_shaderIDs_Order;
    std::unique_ptr<VertexArray> m_screenQuad;
    std::unique_ptr<PostProcessingQueue> m_postprocessingQueue;
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
    void SwitchCubemap(const Cubemap* cubemap);

    GLuint GetColorBufferTextureID() const;

private:
    void AddEntityUID(ECS::EntityUID entity_UID) override;
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