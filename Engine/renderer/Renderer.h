#pragma once

#include <glm/glm.hpp>
#include <Envy/EnvyInstance.h>
#include <Envy/opengl/VertexArray.h>

#include "core/Types.h"
#include "editor/Material.h"
#include "editor/Transform.h"
#include "editor/Camera.h"

HNCRSP_NAMESPACE_START

const uint32_t UBO_BINDING_INDEX_GLOBAL = 0;
const uint32_t UBO_BINDING_INDEX_LIGHT = 1;

// The Renderer can be the interface between the scene and shaders.
// Meaning update in scene's lights are reflected in the shaders via the Renderer.
const uint32_t MAX_POINT_LIGHTS = 20;

struct RenderCommand
{
    GLResource<Envy::VertexArray> vertexArray;
    const Envy::VAOChunk* vaoChunk = nullptr;
    const Material* material = nullptr;
    const Transform* transform = nullptr;
};

struct RenderCommandIndirect
{
    const GLResource<Envy::VertexArray> vertexArray;
    const GLResource<Envy::IndirectBuffer> indirectBuffer;
    const Material* material = nullptr;
    const Transform* transform = nullptr;
};

struct GlobalUBO
{
    float time;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec4 cameraPos;
};

class Renderer
{
public:
    Renderer(const Envy::EnvyInstance* envy_instance);
    ~Renderer();

    void BeginFrame(const Camera* camera);
    void EndFrame(const GLResource<Envy::Cubemap>& cubemap);
    void Render(const RenderCommand& render_command) const;
    void RenderMultiple(const std::vector<RenderCommand>& render_commands) const;
    void RenderIndirect(const RenderCommandIndirect& render_command_indirect) const;

private:
    void _UpdateUBOs(const Camera* camera) const;
    void _RenderToScreenQuad() const;

private:
    const Envy::EnvyInstance* m_envyInstance = nullptr;
    GLResource<Envy::UniformBuffer> m_globalUBO;
    GLResource<Envy::UniformBuffer> m_lightUBO;
    GLResource<Envy::Framebuffer> m_fbo;
    GLResource<Envy::VertexArray> m_screenQuadVAO;

    bool m_canRenderFlag = false;
};

HNCRSP_NAMESPACE_END