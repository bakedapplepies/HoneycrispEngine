#include "renderer/meshes/Cube.h"

#include "util/Path.h"

HNCRSP_NAMESPACE_START

Cube::Cube(const Envy::EnvyInstance* envy_instance)
{
    std::array<Envy::Vertex, 24> vertices = {
        Envy::Vertex {  // front
            .position = glm::vec3(-0.5f,  0.5f,  0.5f),
            .normal   = glm::vec3( 0.0f,  0.0f,  1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f,  0.5f,  0.5f),
            .normal   = glm::vec3( 0.0f,  0.0f,  1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f, -0.5f,  0.5f),
            .normal   = glm::vec3( 0.0f,  0.0f,  1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-0.5f, -0.5f,  0.5f),
            .normal   = glm::vec3( 0.0f,  0.0f,  1.0f)
        },
        Envy::Vertex {  // left
            .position = glm::vec3( 0.5f,  0.5f,  0.5f),
            .normal   = glm::vec3( 1.0f,  0.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f,  0.5f, -0.5f),
            .normal   = glm::vec3( 1.0f,  0.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f, -0.5f, -0.5f),
            .normal   = glm::vec3( 1.0f,  0.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f, -0.5f,  0.5f),
            .normal   = glm::vec3( 1.0f,  0.0f,  0.0f)
        },
        Envy::Vertex {  // back
            .position = glm::vec3( 0.5f,  0.5f, -0.5f),
            .normal   = glm::vec3( 0.0f,  0.0f,  -1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-0.5f,  0.5f, -0.5f),
            .normal   = glm::vec3( 0.0f,  0.0f,  -1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-0.5f, -0.5f, -0.5f),
            .normal   = glm::vec3( 0.0f,  0.0f,  -1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f, -0.5f, -0.5f),
            .normal   = glm::vec3( 0.0f,  0.0f,  -1.0f)
        },
        Envy::Vertex {  // right
            .position = glm::vec3(-0.5f,  0.5f, -0.5f),
            .normal   = glm::vec3(-1.0f,  0.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-0.5f,  0.5f,  0.5f),
            .normal   = glm::vec3(-1.0f,  0.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-0.5f, -0.5f,  0.5f),
            .normal   = glm::vec3(-1.0f,  0.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-0.5f, -0.5f, -0.5f),
            .normal   = glm::vec3(-1.0f,  0.0f,  0.0f)
        },
        Envy::Vertex {  // up
            .position = glm::vec3(-0.5f,  0.5f, -0.5f),
            .normal   = glm::vec3( 0.0f,  1.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f,  0.5f, -0.5f),
            .normal   = glm::vec3( 0.0f,  1.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f,  0.5f,  0.5f),
            .normal   = glm::vec3( 0.0f,  1.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-0.5f,  0.5f,  0.5f),
            .normal   = glm::vec3( 0.0f,  1.0f,  0.0f)
        },
        Envy::Vertex {  // down
            .position = glm::vec3(-0.5f, -0.5f,  0.5f),
            .normal   = glm::vec3( 0.0f, -1.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f, -0.5f,  0.5f),
            .normal   = glm::vec3( 0.0f, -1.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 0.5f, -0.5f, -0.5f),
            .normal   = glm::vec3( 0.0f, -1.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-0.5f, -0.5f, -0.5f),
            .normal   = glm::vec3( 0.0f, -1.0f,  0.0f)
        }
    };
    std::array<GLuint, 36> indices = {
        0, 1, 2,  // front
        0, 2, 3,
        4, 5, 6,  // left
        4, 6, 7,
        8, 9, 10,  // back
        8, 10, 11,
        12, 13, 14,  // right
        12, 14, 15,
        16, 17, 18,  // up
        16, 18, 19,
        20, 21, 22,  // down
        20, 22, 23
    };
    m_vao = envy_instance->CreateVertexArray(vertices.data(),
                                             vertices.size(),
                                             indices.data(),
                                             indices.size());

    const Envy::ShaderProgram* vertexShader =
        envy_instance->GetShaderProgram(Path("engine/renderer/shaders/screen_quad.vert").Str());
    const Envy::ShaderProgram* fragmentShader =
        envy_instance->GetShaderProgram(Path("engine/renderer/shaders/deferred_shading.frag").Str());

    material.pipeline = envy_instance->CreatePipeline();
    material.pipeline->SetVertexProgram(vertexShader);
    material.pipeline->SetFragmentProgram(fragmentShader);
}

RenderCommand Cube::GetRenderCmd() const
{
    constexpr static Envy::VAOChunk vaoChunk = Envy::VAOChunk {
        .elementsOffset = 0,
        .elementsCount = 6,
        .vertexOffset = 0
    };
    constexpr static Transform defaultTransform;

    return RenderCommand {
        .vertexArray = m_vao,
        .vaoChunk = &vaoChunk,
        .material = &material,
        .transform = &defaultTransform
    };
}

HNCRSP_NAMESPACE_END