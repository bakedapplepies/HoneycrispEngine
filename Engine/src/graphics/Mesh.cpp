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

    bool hasColorBit = colors != nullptr;
    bool hasNormalBit = normals != nullptr;
    bool hasUVBit = uvs != nullptr;
    bool hasTangentBit = tangents != nullptr;

    uint16_t vertex_attrib_bits = 
        VERTEX_ATTRIB_POSITION_BIT
        | VERTEX_ATTRIB_COLOR_BIT   * static_cast<uint16_t>(hasColorBit)
        | VERTEX_ATTRIB_NORMAL_BIT  * static_cast<uint16_t>(hasNormalBit)
        | VERTEX_ATTRIB_UV_BIT      * static_cast<uint16_t>(hasUVBit)
        | VERTEX_ATTRIB_TANGENT_BIT * static_cast<uint16_t>(hasTangentBit);

    std::vector<Vertex> vertices;
    vertices.reserve(positions->size());
    for (uint32_t i = 0; i < positions->size(); i++)
    {
        vertices.push_back(Vertex {
            .position = (*positions)[i],
            .color    = (hasColorBit)   ? (*colors)[i]   : glm::vec3(0.0f, 0.0f, 0.0f),
            .normal   = (hasNormalBit)  ? (*normals)[i]  : glm::vec3(0.0f, 0.0f, 0.0f),
            .uv       = (hasUVBit)      ? (*uvs)[i]      : glm::vec2(0.0f, 0.0f),
            .tangent  = (hasTangentBit) ? (*tangents)[i] : glm::vec3(0.0f, 0.0f, 0.0f),
        });
    }

    m_VAO = VertexArray(vertex_attrib_bits, &vertices, indices);
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