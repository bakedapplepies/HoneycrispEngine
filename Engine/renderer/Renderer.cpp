#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include "util/Path.h"

Renderer::Renderer(const Envy::EnvyInstance* envy_instance)
{
    m_envyInstance->SetFrontFaceOrder(Envy::FaceOrder::CLOCKWISE);
    m_envyInstance->SetDepthTesting(true);

    m_envyInstance = envy_instance;

    m_globalUBO = m_envyInstance->CreateUBO(160, 0);

    m_fbo = m_envyInstance->CreateFramebuffer(1280, 720);

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

void Renderer::Render(const Camera* camera, 
                      const GLResource<Envy::Cubemap>& cubemap,
                      const RenderCommand& render_command) const
{
    m_fbo->Bind();
    m_envyInstance->ClearBuffer();

    const Material* material = render_command.material;

    const uint32_t TEXTURE_UNIT_ALBEDO = 0;
    material->albedo->Bind(TEXTURE_UNIT_ALBEDO);

    material->pipeline->Bind();
    material->pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                        render_command.transform->GetModelMatrix());

    // Update only once
    _UpdateGlobalUBO(camera);

    render_command.vertexArray->Bind();

    m_envyInstance->Draw(*render_command.vaoChunk);

    if (cubemap.Usable())  // Rendered last to avoid over-draw
    {
        cubemap->Draw();
    }

    _RenderToScreenQuad();
}

void Renderer::RenderIndirect(const Camera* camera,
                              const GLResource<Envy::Cubemap>& cubemap,
                              const RenderCommandIndirect& render_command_indirect) const
{
    m_fbo->Bind();
    m_envyInstance->ClearBuffer();
    m_envyInstance->SetViewport(0, 0, 1280, 720);

    const Material* material = render_command_indirect.material;

    const uint32_t TEXTURE_UNIT_ALBEDO = 0;
    material->albedo->Bind(TEXTURE_UNIT_ALBEDO);

    material->pipeline->Bind();
    material->pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                        render_command_indirect.transform->GetModelMatrix());

    // Update only once
    _UpdateGlobalUBO(camera);

    render_command_indirect.vertexArray->Bind();
    render_command_indirect.indirectBuffer->Bind();

    m_envyInstance->DrawIndirect(render_command_indirect.indirectBuffer->GetCommandCount());

    if (cubemap.Usable())  // Rendered last to avoid over-draw
    {
        cubemap->Draw();
    }

    _RenderToScreenQuad();
}

void Renderer::_UpdateGlobalUBO(const Camera* camera) const
{
    float time = glfwGetTime();
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::mat4 projectionMatrix = camera->GetProjectionMatrix(16.0f/9.0f);
    m_globalUBO->UploadData(0, 4, &time);
    m_globalUBO->UploadData(16, 64, &viewMatrix);
    m_globalUBO->UploadData(80, 64, &projectionMatrix);
    m_globalUBO->UploadData(144, 16, &camera->position);
}

void Renderer::_RenderToScreenQuad() const
{
    static const Envy::ShaderProgram* vertexShader =
        m_envyInstance->GetShaderProgram(Path("engine/shaders/screen_quad.vert").Str());
    static const Envy::ShaderProgram* fragmentShader =
        m_envyInstance->GetShaderProgram(Path("engine/shaders/screen_quad.frag").Str());
    static const GLResource<Envy::Pipeline>
        quadPipeline = m_envyInstance->CreatePipeline(vertexShader,
                                                      fragmentShader);
    
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