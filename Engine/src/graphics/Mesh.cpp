#include "api/graphics/Mesh.h"
#include "api/components/DrawData.h"
#include "api/ecs/ECSManager.h"
#include "api/managers/ShaderManager.h"

HNCRSP_NAMESPACE_START

Mesh::Mesh(
    std::vector<glm::vec3>* positions,
    std::vector<GLuint>* indices,
    std::vector<glm::vec3>* normals,
    std::vector<glm::vec3>* colors,
    std::vector<glm::vec2>* uvs,
    std::vector<glm::vec3>* tangents
) : m_VAO()
{
    if (positions == nullptr) HNCRSP_TERMINATE("Positions data empty.");
    if (indices == nullptr) HNCRSP_TERMINATE("Indices data empty.");

    std::vector<Vertex> vertices;
    vertices.reserve(positions->size());
    for (uint32_t i = 0; i < positions->size(); i++)
    {
        vertices.push_back(Vertex {
            .position = (*positions)[i],
            .color    = (colors != nullptr)   ? (*colors)[i]   : glm::vec3(0.0f, 0.0f, 0.0f),
            .normal   = (normals != nullptr)  ? (*normals)[i]  : glm::vec3(0.0f, 0.0f, 0.0f),
            .uv       = (uvs != nullptr)      ? (*uvs)[i]      : glm::vec2(0.0f, 0.0f),
            .tangent  = (tangents != nullptr) ? (*tangents)[i] : glm::vec3(0.0f, 0.0f, 0.0f),
        });
    }
}

Mesh::Mesh(
    uint16_t vertex_attrib_bits,
    const std::vector<float>& vertex_data,
    const std::vector<GLuint>& indices_data
) : m_VAO(vertex_attrib_bits, vertex_data, indices_data)
{}

Mesh::Mesh(Mesh&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);

    return *this;
}

void Mesh::virt_AddDrawDataToRenderer(ECS::EntityUID entityUID, const Material& material) const
{
    DrawData drawData;
    drawData.VAO_id = m_VAO.GetID();
    drawData.materials.push_back(material);
    drawData.meta_data.push_back(MeshMetaData {
        .mesh_vertex_offset = 0,
        .indices_buffer_count = static_cast<GLuint>(m_VAO.GetIndicesLen()),
        .material_index = 0
    });

    g_ECSManager.AddComponent<DrawData>(entityUID, drawData);
}

HNCRSP_NAMESPACE_END