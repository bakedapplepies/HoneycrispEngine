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

Mesh::Mesh(const Mesh& other)
{
    m_VAO = other.m_VAO;
    m_numVertices = other.m_numVertices;
    m_relativeOrigin = other.m_relativeOrigin;
}

Mesh& Mesh::operator=(const Mesh& other)
{
    m_VAO = other.m_VAO;
    m_numVertices = other.m_numVertices;
    m_relativeOrigin = other.m_relativeOrigin;

    return *this;
}

Mesh::Mesh(Mesh&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
    m_numVertices = other.m_numVertices;
    other.m_numVertices = 0;
    m_relativeOrigin = std::move(other.m_relativeOrigin);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
    m_numVertices = other.m_numVertices;
    other.m_numVertices = 0;
    m_relativeOrigin = std::move(other.m_relativeOrigin);

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

HNCRSP_NAMESPACE_END