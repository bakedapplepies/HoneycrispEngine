#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include "util/Path.h"
#include "core/Constants.h"

HNCRSP_NAMESPACE_START

Renderer::Renderer(const Envy::EnvyInstance* envy_instance)
{
    m_envyInstance->SetFrontFaceOrder(Envy::FaceOrder::CLOCKWISE);
    m_envyInstance->SetDepthTesting(true);

    m_envyInstance = envy_instance;

    m_globalUBO = m_envyInstance->CreateUBO(160, UBO_BINDING_INDEX_GLOBAL);
    m_lightUBO = m_envyInstance->CreateUBO(16 + 20 * 16, UBO_BINDING_INDEX_LIGHT);

    uint32_t pointLightNum = 1;
    m_lightUBO->UploadData(0, 4, &pointLightNum);

    m_fbo = m_envyInstance->CreateFramebuffer(2560, 1440);

    std::array<Envy::Vertex, 4> vertices = {
        Envy::Vertex {
            .position = glm::vec3(-1.0f,  1.0f,  0.0f),
            .normal   = glm::vec3( 0.0f,  0.0f,  0.0f),
            .uv       = glm::vec2( 0.0f,  1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 1.0f,  1.0f,  0.0f),
            .normal   = glm::vec3( 0.0f,  0.0f,  0.0f),
            .uv       = glm::vec2( 1.0f,  1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 1.0f, -1.0f,  0.0f),
            .normal   = glm::vec3( 0.0f,  0.0f,  0.0f),
            .uv       = glm::vec2( 1.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-1.0f, -1.0f,  0.0f),
            .normal   = glm::vec3( 0.0f,  0.0f,  0.0f),
            .uv       = glm::vec2( 0.0f,  0.0f)
        }
    };
    std::array<GLuint, 6> indices = {
        0, 1, 2,
        0, 2, 3
    };
    m_screenQuadVAO = m_envyInstance->CreateVertexArray(vertices.data(),
                                                        vertices.size(),
                                                        indices.data(),
                                                        indices.size());
}

Renderer::~Renderer()
{
    m_envyInstance = nullptr;
    m_globalUBO = GLResource<Envy::UniformBuffer>::empty;
    m_fbo = GLResource<Envy::Framebuffer>::empty;
    m_screenQuadVAO = GLResource<Envy::VertexArray>::empty;
}

void Renderer::BeginFrame(const Camera* camera)
{
    m_canRenderFlag = true;

    m_fbo->Bind();
    m_envyInstance->ClearBuffer();
    m_envyInstance->SetViewport(0, 0, 2560, 1440);

    // Update only once per frame
    _UpdateUBOs(camera);
}

void Renderer::EndFrame(const GLResource<Envy::Cubemap>& cubemap)
{
    m_canRenderFlag = false;

    if (cubemap.Usable())  // Rendered last to avoid over-draw
    {
        cubemap->Draw();
    }
    _RenderToScreenQuad();
}

void Renderer::Render(const RenderCommand& render_command) const
{
    if (!m_canRenderFlag)
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

void Renderer::RenderMultiple(const std::vector<RenderCommand>& render_commands) const
{
    if (!m_canRenderFlag)
    {
        fmt::println("Honeycrisp: Frame hasn't began (use BeginFrame().)");
        return;
    }

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

void Renderer::RenderIndirect(const RenderCommandIndirect& render_command_indirect) const
{
    if (!m_canRenderFlag)
    {
        fmt::println("Frame hasn't began (use BeginFrame().)");
        return;
    }    

    const Material* material = render_command_indirect.material;
    const uint32_t TEXTURE_UNIT_ALBEDO = 0;
    material->albedo->Bind(TEXTURE_UNIT_ALBEDO);
    material->pipeline->Bind();
    material->pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                        render_command_indirect.transform->GetModelMatrix());

    render_command_indirect.vertexArray->Bind();
    render_command_indirect.indirectBuffer->Bind();
    m_envyInstance->DrawIndirect(render_command_indirect.indirectBuffer->GetCommandCount());
}

void Renderer::_UpdateUBOs(const Camera* camera) const
{
    float time = glfwGetTime();
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::mat4 projectionMatrix = camera->GetProjectionMatrix(16.0f/9.0f);
    m_globalUBO->UploadData(0, 4, &time);
    m_globalUBO->UploadData(16, 64, &viewMatrix);
    m_globalUBO->UploadData(80, 64, &projectionMatrix);
    m_globalUBO->UploadData(144, 16, &camera->position);

    m_lightUBO->UploadData(16, 16, &camera->position);
}

void Renderer::_RenderToScreenQuad() const
{
    static const Envy::ShaderProgram* vertexShader =
        m_envyInstance->GetShaderProgram(Path("engine/renderer/shaders/screen_quad.vert").Str());
    static const Envy::ShaderProgram* fragmentShader =
        m_envyInstance->GetShaderProgram(Path("engine/renderer/shaders/screen_quad.frag").Str());
    static const GLResource<Envy::Pipeline>
        quadPipeline = m_envyInstance->CreatePipeline(vertexShader, fragmentShader);
    
    m_envyInstance->BindDefaultFramebuffer();
    m_screenQuadVAO->Bind();
    quadPipeline->Bind();
    m_fbo->BindColorAttachment(0);
            
    static Envy::VAOChunk quadVAOChunk
    {
        .elementsOffset = 0,
        .elementsCount = 6, 
        .vertexOffset = 0
    };

    m_envyInstance->SetViewport(0, 0, 2560, 1440);
    m_envyInstance->ClearBuffer();
    m_envyInstance->Draw(quadVAOChunk);
}

HNCRSP_NAMESPACE_END