#include "Mesh.h"
#include "src/components/MeshData.h"
#include "src/core/ShaderManager.h"
#include "src/core/SceneManager.h"


HNCRSP_NAMESPACE_START

Mesh::Mesh(
    std::vector<glm::vec3>* vertices,
    std::vector<GLuint>* indices,
    std::vector<glm::vec3>* normals,
    std::vector<glm::vec3>* colors,
    std::vector<glm::vec2>* uvs
) {
    m_VAO = std::make_shared<VertexArray>(
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
    const float* vertex_data,
    size_t vertex_data_len,
    const GLuint* indices_data,
    size_t indices_data_len
) {
    m_VAO = std::make_shared<VertexArray>(
        vertex_attrib_bits,
        vertex_data,
        vertex_data_len,
        indices_data,
        indices_data_len
    );
    m_numVertices = indices_data_len;
}

Mesh::Mesh(const Mesh& other)
{
    m_VAO = other.m_VAO;
    m_numVertices = other.m_numVertices;
    // m_relativeOrigin = other.m_relativeOrigin;
}

Mesh& Mesh::operator=(const Mesh& other)
{
    m_VAO = other.m_VAO;
    m_numVertices = other.m_numVertices;
    // // m_relativeOrigin = other.m_relativeOrigin;

    return *this;
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

void Mesh::virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material)
{
    MeshData meshData;
    meshData.VAO_id = m_VAO->getID();
    meshData.num_vertices = m_numVertices;
    if (material)
    {
        meshData.material = material;
    }
    else
    {
        meshData.material = std::make_shared<Material>(g_ShaderManager.basicShader);
    }

    g_ECSManager->AddComponent<MeshData>(entityUID, meshData);
}

const VertexArray* Mesh::GetVAO() const
{
    return m_VAO.get();
}

HNCRSP_NAMESPACE_END