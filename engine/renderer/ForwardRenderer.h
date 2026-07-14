#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"

HNCRSP_NAMESPACE_START

class ForwardRenderer : public Renderer
{
public:
    ForwardRenderer(const Envy::EnvyInstance* envy_instance);
    ~ForwardRenderer();

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

private:
    const Envy::EnvyInstance* m_envyInstance = nullptr;
    GLResource<Envy::Framebuffer> m_mainFBO;
    GLResource<Envy::Framebuffer> m_shadowFBO;
    Material m_shadowMappingMaterial;

    FrameType m_currentFrameType = FrameType::NONE;
};

HNCRSP_NAMESPACE_END