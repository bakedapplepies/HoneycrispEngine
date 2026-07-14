#pragma once

#include <Envy/EnvyInstance.h>
#include <Envy/opengl/VertexArray.h>

#include "core/Types.h"
#include "Transform.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"

HNCRSP_NAMESPACE_START

// The Renderer can be the interface between the scene and shaders.
// Meaning update in scene's lights are reflected in the shaders via the Renderer.
constexpr uint32_t MAX_POINT_LIGHTS = 20;
constexpr uint32_t MAIN_FBO_COLOR_ATTACHMENT_INDEX = 0;
constexpr uint32_t SHADOW_FBO_DEPTH_ATTACHMENT_INDEX = 0;

enum class FrameType
{
    NORMAL,
    SHADOW,
    NONE
};

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

struct FrameData
{
    FrameType frameType;
    GLResource<Envy::Framebuffer> framebuffer;
    Envy::FBOBufferFlags fboClearBufferFlags;
    const Camera* camera;
    CameraProjection cameraProjection;
};

struct DirLight
{
    alignas(16) glm::vec3 direction;
};

struct PointLight
{
    alignas(16) glm::vec3 position;
};

struct GlobalUBO
{
    alignas(16) glm::mediump_float32 u_time;
    alignas(16) glm::mat4 u_view;
    alignas(16) glm::mat4 u_projection;
    alignas(16) glm::vec4 u_cameraPos;
    alignas(16) glm::mat4 u_lightSpace;
};

struct LightUBO
{
    DirLight u_dirLight;
    uint32_t u_numPointLight;
    PointLight u_pointLights[20];
};

struct MaterialUBO
{
    float u_ambient_intensity;
    float u_diffuse_intensity;
    float u_specular_intensity;
    float u_roughness_scalar;
};

class Renderer
{
public:
    Renderer(const Envy::EnvyInstance* envy_instance);
    virtual ~Renderer();

    // UBO Functions
    virtual void UpdateDirLight(const DirLight& dir_light) const = 0;
    virtual void UpdatePointLights(const std::vector<PointLight>& point_lights) const = 0;
    virtual void UpdateGlobalMatParam(const MaterialUBO& mat_params) const = 0;

    // Rendering Functions
    virtual void BeginFrame(const FrameData& frame_data) = 0;
    virtual GLResource<Envy::Texture2D> EndFrame(const GLResource<Envy::Cubemap>& cubemap = {}) = 0;
    virtual void Render(const RenderCommand& render_command) const = 0;
    virtual void RenderMultiple(const std::vector<RenderCommand>& render_commands) const = 0;
    virtual void RenderIndirect(const RenderCommandIndirect& render_command_indirect) const = 0;

    // Utility Functions
    virtual GLResource<Envy::Framebuffer> GetMainFramebuffer() const = 0;
    virtual GLResource<Envy::Framebuffer> GetMainShadowFramebuffer() const = 0;

protected:
    GLResource<Envy::UniformBuffer> m_globalUBO;
    GLResource<Envy::UniformBuffer> m_lightUBO;
    GLResource<Envy::UniformBuffer> m_materialUBO;

private:
    static bool m_rendererSetup;
};

HNCRSP_NAMESPACE_END