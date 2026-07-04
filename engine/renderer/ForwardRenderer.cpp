#include "ForwardRenderer.h"

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <tracy/Tracy.hpp>

#include "core/Constants.h"
#include "util/Path.h"

HNCRSP_NAMESPACE_START

ForwardRenderer::ForwardRenderer(const Envy::EnvyInstance* envy_instance)
{
    m_envyInstance->SetFrontFaceOrder(Envy::FaceOrder::CLOCKWISE);
    m_envyInstance->SetDepthTesting(true);

    m_envyInstance = envy_instance;

    /*                      size    offset  total_size
        float u_time      : 4       0
        mat4 u_view       : 64      16
        mat4 u_projection : 64      80
        vec4 u_cameraPos  : 16      144
        mat4 u_lightSpace : 64      160     224
    */
    m_globalUBO = m_envyInstance->CreateUBO(224, UBO_BINDING_INDEX_GLOBAL);

    /*                                               size    offset  total_size
        DirLight u_light                           : 16      0
        uint u_numPointLight                       : 4       16
        PointLight u_pointLights[MAX_POINT_LIGHTS] : 320     32      352
    */
    m_lightUBO = m_envyInstance->CreateUBO(352, UBO_BINDING_INDEX_LIGHT);

    /*                               size    offset  total_size
        float u_ambient_intensity  : 4       0
        float u_diffuse_intensity  : 4       16
        float u_specular_intensity : 4       32
        float u_roughness_scalar   : 4       48      52
    */
    m_materialUBO = m_envyInstance->CreateUBO(64, UBO_BINDING_INDEX_MATERIAL);

    uint32_t pointLightNum = 1;
    m_lightUBO->UploadData(16, 4, &pointLightNum);

    // TODO: In case a Resource container is assigned to a different resource
    // whilst already holding a resource, it may lead to non-deallocated memory.
    // It will get cleaned up when the program ends, but in the duration the
    // program is running, it's just sitting there.
    m_mainFBO = m_envyInstance->CreateFramebuffer(2560, 1440, {
        Envy::FBOAttachment {
            .target = Envy::FBOAttachmentTarget::COLOR0,
            .usage = Envy::FBOAttachmentUsage::TEXTURE,
            .format = Envy::TextureFormat::RGBA8
        },
        Envy::FBOAttachment {
            .target = Envy::FBOAttachmentTarget::DEPTH,
            .usage = Envy::FBOAttachmentUsage::RENDER_BUFFER,
            .format = Envy::TextureFormat::DEPTH
        }
    });
    m_shadowFBO = m_envyInstance->CreateFramebuffer(2560, 1440, {
        Envy::FBOAttachment {
            .target = Envy::FBOAttachmentTarget::DEPTH,
            .usage = Envy::FBOAttachmentUsage::TEXTURE,
            .format = Envy::TextureFormat::DEPTH32F
        }
    });
    m_shadowFBO->EnableColorTargetRead(Envy::FBOAttachmentTarget::NONE);
    m_shadowFBO->EnableColorTargetWrite(Envy::FBOAttachmentTarget::NONE);

    m_shadowMappingMaterial.pipeline = m_envyInstance->CreatePipeline();
    m_shadowMappingMaterial.pipeline->SetVertexProgram(
        m_envyInstance->GetShaderProgram(Path("engine/renderer/shaders/shadow.vert").Str()));
    m_shadowMappingMaterial.pipeline->SetFragmentProgram(
        m_envyInstance->GetShaderProgram(Path("engine/renderer/shaders/shadow.frag").Str()));
}

ForwardRenderer::~ForwardRenderer()
{
    m_envyInstance = nullptr;
    m_globalUBO = GLResource<Envy::UniformBuffer>::empty;
    m_lightUBO = GLResource<Envy::UniformBuffer>::empty;
    m_materialUBO = GLResource<Envy::UniformBuffer>::empty;
    m_mainFBO = GLResource<Envy::Framebuffer>::empty;
    m_shadowFBO = GLResource<Envy::Framebuffer>::empty;
}

void ForwardRenderer::UpdateDirLight(const DirLight& dir_light) const
{
    m_lightUBO->UploadData(offsetof(LightUBO, u_dirLight), 16, &dir_light);
}

void ForwardRenderer::UpdatePointLights(const std::vector<PointLight>& point_lights) const
{
    // m_pointLights = std::move(point_lights);

    uint32_t pointLightNum = point_lights.size();
    m_lightUBO->UploadData(offsetof(LightUBO, u_numPointLight), 4, &pointLightNum);
    for (uint32_t i = 0; i < point_lights.size(); i++)
    {
        m_lightUBO->UploadData(offsetof(LightUBO, u_pointLights) + 16 * i, 16, point_lights.data() + i);
    }
}

void ForwardRenderer::UpdateGlobalMatParam(const MaterialUBO& mat_params) const
{
    m_materialUBO->UploadData(offsetof(MaterialUBO, u_ambient_intensity), 4, &mat_params.u_ambient_intensity);
    m_materialUBO->UploadData(offsetof(MaterialUBO, u_diffuse_intensity), 4, &mat_params.u_diffuse_intensity);
    m_materialUBO->UploadData(offsetof(MaterialUBO, u_specular_intensity), 4, &mat_params.u_specular_intensity);
    m_materialUBO->UploadData(offsetof(MaterialUBO, u_roughness_scalar), 4, &mat_params.u_roughness_scalar);
}

void ForwardRenderer::BeginFrame(const FrameData& frame_data)
{
    assert(m_currentFrameType == FrameType::NONE && "ForwardRenderer::BeginFrame: In the middle of a frame.");

    m_currentFrameType = frame_data.frameType;

    frame_data.framebuffer->Bind();
    if ((frame_data.fboClearBufferFlags & Envy::FBOBuffer::COLOR) == Envy::FBOBuffer::COLOR)
        frame_data.framebuffer->ClearBuffer(Envy::FBOBuffer::COLOR, 0.0f);
    if ((frame_data.fboClearBufferFlags & Envy::FBOBuffer::DEPTH) == Envy::FBOBuffer::DEPTH)
        frame_data.framebuffer->ClearBuffer(Envy::FBOBuffer::DEPTH, 1.0f);
    if ((frame_data.fboClearBufferFlags & Envy::FBOBuffer::STENCIL) == Envy::FBOBuffer::STENCIL)
        frame_data.framebuffer->ClearBuffer(Envy::FBOBuffer::STENCIL, 0);
    m_envyInstance->SetViewport(0, 0, 2560, 1440);

    if (m_currentFrameType == FrameType::NORMAL)
        _UpdateUBOCamera(frame_data.camera, frame_data.cameraProjection);
    else if (m_currentFrameType == FrameType::SHADOW)
        _UpdateUBODepthCamera(frame_data.camera, frame_data.cameraProjection);
    _UpdateUBOTime();
}

GLResource<Envy::Texture2D> ForwardRenderer::EndFrame(const GLResource<Envy::Cubemap>& cubemap)
{
    assert(m_currentFrameType != FrameType::NONE && "ForwardRenderer::EndFrame: Frame hasn't started.");

    FrameType frameType = m_currentFrameType;
    m_currentFrameType = FrameType::NONE;
    if (frameType == FrameType::NORMAL)
    {
        if (cubemap.Usable())  // Rendered last to avoid over-draw
        {
            cubemap->Draw();
        }
        return m_mainFBO->GetTex2DAttachment(MAIN_FBO_COLOR_ATTACHMENT_INDEX);
    }
    else if (frameType == FrameType::SHADOW)
    {
        return m_shadowFBO->GetTex2DAttachment(SHADOW_FBO_DEPTH_ATTACHMENT_INDEX);
    }
    assert(false && "ForwardRenderer::EndFrame: Unknown frame type.");
}

void ForwardRenderer::Render(const RenderCommand& render_command) const
{
    ZoneScoped;
    if (m_currentFrameType == FrameType::NONE)
    {
        fmt::println("Honeycrisp: Frame hasn't began (use BeginFrame().)");
        return;
    }

    const Material* material = render_command.material;
    const uint32_t TEXTURE_UNIT_ALBEDO = 0;
    material->albedo->Bind(TEXTURE_UNIT_ALBEDO);
    material->roughness->Bind(TEXTURE_UNIT_ROUGHNESS);
    material->pipeline->Bind();
    material->pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                        render_command.transform->GetModelMatrix());

    render_command.vertexArray->Bind();
    m_envyInstance->Draw(*render_command.vaoChunk);
}

void ForwardRenderer::RenderMultiple(const std::vector<RenderCommand>& render_commands) const
{
    ZoneScoped;
    if (m_currentFrameType == FrameType::NONE)
    {
        fmt::println("Honeycrisp: Frame hasn't began (use BeginFrame().)");
        return;
    }

    if (m_currentFrameType == FrameType::NORMAL)
    {
        for (uint32_t i = 0; i < render_commands.size(); i++)
        {
            const Material* material = render_commands[i].material;
            material->albedo->Bind(TEXTURE_UNIT_ALBEDO);
            if (material->roughness.Usable()) material->roughness->Bind(TEXTURE_UNIT_ROUGHNESS);
            material->pipeline->Bind();
            material->pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                                render_commands[i].transform->GetModelMatrix());
    
            render_commands[i].vertexArray->Bind();
            m_envyInstance->Draw(*render_commands[i].vaoChunk);
        }
    }
    else if (m_currentFrameType == FrameType::SHADOW)
    {
        m_shadowMappingMaterial.pipeline->Bind();
        for (uint32_t i = 0; i < render_commands.size(); i++)
        {
            m_shadowMappingMaterial.pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                                              render_commands[i].transform->GetModelMatrix());
            render_commands[i].vertexArray->Bind();
            m_envyInstance->Draw(*render_commands[i].vaoChunk);
        }
    }
}

void ForwardRenderer::RenderIndirect(const RenderCommandIndirect& render_command_indirect) const
{
    ZoneScoped;
    if (m_currentFrameType == FrameType::NONE)
    {
        fmt::println("Honeycrisp: Frame hasn't began (use BeginFrame().)");
        return;
    }    

    render_command_indirect.vertexArray->Bind();
    render_command_indirect.indirectBuffer->Bind();
    const Material* material = nullptr;
    if (m_currentFrameType == FrameType::NORMAL)
    {
        material = render_command_indirect.material;
        const uint32_t TEXTURE_UNIT_ALBEDO = 0;
        material->albedo->Bind(TEXTURE_UNIT_ALBEDO);
    }
    else if (m_currentFrameType == FrameType::SHADOW)
    {
        material = &m_shadowMappingMaterial;
    }
    material->pipeline->Bind();
    material->pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                        render_command_indirect.transform->GetModelMatrix());
    
    m_envyInstance->DrawIndirect(render_command_indirect.indirectBuffer->GetCommandCount());
}

GLResource<Envy::Framebuffer> ForwardRenderer::GetMainFramebuffer() const
{
    return m_mainFBO;
}

GLResource<Envy::Framebuffer> ForwardRenderer::GetMainShadowFramebuffer() const
{
    return m_shadowFBO;
}

void ForwardRenderer::_UpdateUBOTime() const
{
    float time = glfwGetTime();
    m_globalUBO->UploadData(offsetof(GlobalUBO, u_time), 4, &time);
}

void ForwardRenderer::_UpdateUBOCamera(const Camera* camera, CameraProjection camera_projection) const
{
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::mat4 projectionMatrix;
    if (camera_projection == CameraProjection::ORTHOGRAPHIC)
        projectionMatrix = camera->GetOrthogonalProjectionMatrix();
    else
        projectionMatrix = camera->GetPerspectiveProjectionMatrix(16.0f/9.0f);

    m_globalUBO->UploadData(offsetof(GlobalUBO, u_view), 64, &viewMatrix);
    m_globalUBO->UploadData(offsetof(GlobalUBO, u_projection), 64, &projectionMatrix);
    m_globalUBO->UploadData(offsetof(GlobalUBO, u_cameraPos), 16, &camera->position);
}

void ForwardRenderer::_UpdateUBODepthCamera(const Camera* camera, CameraProjection camera_projection) const
{
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::mat4 projectionMatrix;
    if (camera_projection == CameraProjection::ORTHOGRAPHIC)
        projectionMatrix = camera->GetOrthogonalProjectionMatrix();
    else
        projectionMatrix = camera->GetPerspectiveProjectionMatrix(16.0f/9.0f);

    glm::mat4 lightSpace = projectionMatrix * viewMatrix;
    m_globalUBO->UploadData(offsetof(GlobalUBO, u_lightSpace), 64, &lightSpace);
}

HNCRSP_NAMESPACE_END