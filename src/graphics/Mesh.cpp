#include "Mesh.h"
#include "src/components/DrawData.h"
#include "src/managers/ShaderManager.h"
#include "src/managers/SceneManager.h"


HNCRSP_NAMESPACE_START

Mesh::Mesh(
    std::vector<glm::vec3>* vertices,
    std::vector<GLuint>* indices,
    std::vector<glm::vec3>* normals,
    std::vector<glm::vec3>* colors,
    std::vector<glm::vec2>* uvs
) {
    m_VAO = std::make_unique<VertexArray>(
        vertices,
        indices,
        normals,
        colors,
        uvs
    );
    m_numVertices = indices->size();
}

Mesh::Mesh(
    unsigned char vertex_attrib_bits,
    const std::vector<float>& vertex_data,
    const std::vector<GLuint>& indices_data
) {
    m_VAO = std::make_unique<VertexArray>(
        vertex_attrib_bits,
        vertex_data,
        indices_data
    );
    m_numVertices = indices_data.size();
}

Mesh::Mesh(Mesh&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
    m_numVertices = other.m_numVertices;
    other.m_numVertices = 0;
    // // m_relativeOrigin = std::move(other.m_relativeOrigin);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
    m_numVertices = other.m_numVertices;
    other.m_numVertices = 0;
    // m_relativeOrigin = std::move(other.m_relativeOrigin);

    return *this;
}

void Mesh::virt_AddDrawDataToRenderer(EntityUID entityUID)
{
    DrawData drawData;
    drawData.VAO_id = m_VAO->getID();
    drawData.meta_data.emplace_back(0, m_numVertices);
    drawData.materials.push_back(std::make_shared<Material>(g_ShaderManager.basicShader));

    g_ECSManager->AddComponent<DrawData>(entityUID, drawData);
}

HNCRSP_NAMESPACE_END