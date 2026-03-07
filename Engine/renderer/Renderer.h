#pragma once

#include <glm/glm.hpp>
#include <Envy/EnvyInstance.h>
#include <Envy/opengl/VertexArray.h>

#include "core/Types.h"
#include "editor/Material.h"
#include "editor/Transform.h"
#include "editor/Camera.h"

struct RenderCommand
{
    const GLResource<Envy::VertexArray> vertexArray;
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

    void Render(const Camera* camera,
                const GLResource<Envy::Cubemap>& cubemap,
                const RenderCommand& render_command) const;
    void RenderIndirect(const Camera* camera,
                        const GLResource<Envy::Cubemap>& cubemap,
                        const RenderCommandIndirect& render_command_indirect) const;

private:
    void _UpdateGlobalUBO(const Camera* camera) const;
    void _RenderToScreenQuad() const;

private:
    const Envy::EnvyInstance* m_envyInstance = nullptr;
    GLResource<Envy::UniformBuffer> m_globalUBO;
    GLResource<Envy::Framebuffer> m_fbo;
    GLResource<Envy::VertexArray> m_screenQuadVAO;
};