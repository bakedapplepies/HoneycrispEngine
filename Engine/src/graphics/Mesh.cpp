#include "api/graphics/Mesh.h"
#include "api/components/DrawData.h"
#include "api/ecs/ECSManager.h"
#include "api/managers/ShaderManager.h"

HNCRSP_NAMESPACE_START

Mesh::Mesh(
    std::vector<glm::vec3>* vertices,
    std::vector<GLuint>* indices,
    std::vector<glm::vec3>* normals,
    std::vector<glm::vec3>* colors,
    std::vector<glm::vec2>* uvs
) : m_VAO(
    vertices,
    indices,
    normals,
    colors,
    uvs
) {}

Mesh::Mesh(
    uint8_t vertex_attrib_bits,
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

void Mesh::virt_AddDrawDataToRenderer(ECS::EntityUID entityUID) const
{
    DrawData drawData;
    drawData.VAO_id = m_VAO.GetID();
    drawData.materials.push_back(std::make_shared<Material>(g_ShaderManager.basicShader));

    MeshMetaData meshMetaData;
    meshMetaData.mesh_vertex_offset = 0;
    meshMetaData.indices_buffer_count = m_VAO.GetIndicesLen();
    meshMetaData.material_index = 0;
    drawData.meta_data.push_back(meshMetaData);

    g_ECSManager.AddComponent<DrawData>(entityUID, drawData);
}

HNCRSP_NAMESPACE_END