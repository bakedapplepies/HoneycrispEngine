#pragma once

#include <glm/glm.hpp>
#include <Envy/EnvyInstance.h>
#include <Envy/opengl/VertexArray.h>

#include "core/Types.h"
#include "editor/Material.h"
#include "editor/Transform.h"
#include "editor/Camera.h"

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

struct GlobalUBO
{
    alignas(16) glm::mediump_float32 u_time;
    alignas(16) glm::mat4 u_view;
    alignas(16) glm::mat4 u_projection;
    alignas(16) glm::vec4 u_cameraPos;
    alignas(16) glm::mat4 u_lightSpace;
};

struct DirLight
{
    alignas(16) glm::vec3 direction;
};

struct PointLight
{
    alignas(16) glm::vec3 position;
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

struct FrameData
{
    FrameType frameType;
    GLResource<Envy::Framebuffer> framebuffer;
    Envy::FBOBufferFlags fboClearBufferFlags;
    const Camera* camera;
    CameraProjection cameraProjection;
};

class Renderer
{
public:
    Renderer(const Envy::EnvyInstance* envy_instance);
    ~Renderer();

    // UBO Functions
    void UpdateDirLight(const DirLight& dir_light) const;
    void UpdatePointLights(const std::vector<PointLight>& point_lights) const;
    void UpdateGlobalMatParam(const MaterialUBO& mat_params) const;

    // Rendering Functions
    void BeginFrame(const FrameData& frame_data);
    GLResource<Envy::Texture2D> EndFrame(const GLResource<Envy::Cubemap>& cubemap = {});
    void Render(const RenderCommand& render_command) const;
    void RenderMultiple(const std::vector<RenderCommand>& render_commands) const;
    void RenderIndirect(const RenderCommandIndirect& render_command_indirect) const;

    // void Display() const;

    // Utility Functions
    GLResource<Envy::Framebuffer> GetMainFramebuffer() const;
    GLResource<Envy::Framebuffer> GetMainShadowFramebuffer() const;

private:
    void _UpdateUBOTime() const;
    void _UpdateUBOCamera(const Camera* camera, CameraProjection camera_projection) const;
    void _UpdateUBODepthCamera(const Camera* camera, CameraProjection camera_projection) const;
    void _RenderShadowMap() const;

private:
    const Envy::EnvyInstance* m_envyInstance = nullptr;
    GLResource<Envy::UniformBuffer> m_globalUBO;
    GLResource<Envy::UniformBuffer> m_lightUBO;
    GLResource<Envy::UniformBuffer> m_materialUBO;
    GLResource<Envy::Framebuffer> m_mainFBO;
    GLResource<Envy::Framebuffer> m_shadowFBO;
    Material m_shadowMappingMaterial;

    FrameType m_currentFrameType = FrameType::NONE;
};

HNCRSP_NAMESPACE_END