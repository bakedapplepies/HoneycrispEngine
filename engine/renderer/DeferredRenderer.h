#pragma once

#include "core/Defines.h"
#include "Renderer.h"
#include "meshes/Quad.h"

HNCRSP_NAMESPACE_START

class DeferredRenderer : public Renderer
{
public:
    DeferredRenderer(const Envy::EnvyInstance* envy_instance);
    ~DeferredRenderer();

    // UBO Functions
    void UpdateDirLight(const DirLight& dir_light) const override;
    void UpdatePointLights(const std::vector<PointLight>& point_lights) const override;
    void UpdateGlobalMatParam(const MaterialUBO& mat_params) const override;

    // Rendering Functions
    void BeginFrame(const FrameData& frame_data) override;
    GLResource<Envy::Texture2D> EndFrame(const GLResource<Envy::Cubemap>& cubemap = {}) override;
    void Render(const RenderCommand& render_command) const override;
    void RenderMultiple(const std::vector<RenderCommand>& render_commands) const override;
    void RenderIndirect(const RenderCommandIndirect& render_command_indirect) const override;

    // Utility Functions
    GLResource<Envy::Framebuffer> GetMainFramebuffer() const override;
    GLResource<Envy::Framebuffer> GetMainShadowFramebuffer() const override;

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
    GLResource<Envy::Framebuffer> m_gBuffer;
    GLResource<Envy::Framebuffer> m_mainFBO;
    GLResource<Envy::Framebuffer> m_shadowFBO;
    std::unique_ptr<Quad> m_quad;
    Material m_deferredMaterial;
    Material m_shadowMappingMaterial;

    FrameType m_currentFrameType = FrameType::NONE;
};

HNCRSP_NAMESPACE_END