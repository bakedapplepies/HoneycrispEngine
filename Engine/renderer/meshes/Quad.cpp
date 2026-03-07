#include "renderer/meshes/Quad.h"

#include "util/Path.h"

Quad::Quad(const Envy::EnvyInstance* envy_instance)
{
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
    m_vao = envy_instance->CreateVertexArray(vertices.data(),
                                             vertices.size(),
                                             indices.data(),
                                             indices.size());

    const Envy::ShaderProgram* vertexShader =
        envy_instance->GetShaderProgram(Path("engine/shaders/default.vert").Str());
    const Envy::ShaderProgram* fragmentShader =
        envy_instance->GetShaderProgram(Path("engine/shaders/default.frag").Str());

    material.albedo = envy_instance->GetTexture2D(Path("engine/resources/images/villager.png").Str());
    material.pipeline = envy_instance->CreatePipeline();
    material.pipeline->SetVertexProgram(vertexShader);
    material.pipeline->SetFragmentProgram(fragmentShader);
}

RenderCommand Quad::GetRenderCmd() const
{
    static Envy::VAOChunk vaoChunk = Envy::VAOChunk {
        .elementsOffset = 0,
        .elementsCount = 6,
        .vertexOffset = 0
    };

    return RenderCommand {
        .vertexArray = m_vao,
        .vaoChunk = &vaoChunk,
        .material = &material,
        .transform = &transform
    };
}