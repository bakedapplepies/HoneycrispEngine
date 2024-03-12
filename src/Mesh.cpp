#include "Mesh.h"
#include "src/components/MeshData.h"
#include "src/core/ShaderManager.h"


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

Mesh::Mesh(Mesh&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);

    return *this;
}

void Mesh::virt_AddMeshDataToRenderer(EntityUID entityUID)
{
    MeshData meshData;
    meshData.VAO_id = m_VAO->getID();
    meshData.num_vertices = m_numVertices;
    meshData.material = std::make_shared<Material>(g_ShaderManager.basicShader);

    g_ECSManager->AddComponent<MeshData>(entityUID, meshData);
}

HNCRSP_NAMESPACE_END