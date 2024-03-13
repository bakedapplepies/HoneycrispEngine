#include "Cube.h"
#include "src/core/Texture2DManager.h"
#include "src/core/ShaderManager.h"
#include "src/core/SceneManager.h"


HNCRSP_NAMESPACE_START

Cube::Cube()
{
    InitializeAttributeData();
}

Cube::Cube(Cube&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
}

Cube& Cube::operator=(Cube&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);

    return *this;
}

void Cube::InitializeAttributeData()
{
    std::vector<glm::vec3> verticesPos = std::vector<glm::vec3> {
        // front
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),

        // right
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),

        // back
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),

        // left
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),

        // top
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),

        // bottom
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
    };

    std::vector<glm::vec3> colors = std::vector<glm::vec3> {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),

        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),

        glm::vec3(0.369f, 0.616f, 0.204f),
        glm::vec3(0.369f, 0.616f, 0.204f),
        glm::vec3(0.369f, 0.616f, 0.204f),
        glm::vec3(0.369f, 0.616f, 0.204f),

        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
    };

    std::vector<glm::vec3> normals = std::vector<glm::vec3> {
        // front
        glm::vec3(0.0f,  0.0f,  1.0f), 
        glm::vec3(0.0f,  0.0f,  1.0f), 
        glm::vec3(0.0f,  0.0f,  1.0f), 
        glm::vec3(0.0f,  0.0f,  1.0f), 
        
        // right
        glm::vec3(1.0f,  0.0f,  0.0f), 
        glm::vec3(1.0f,  0.0f,  0.0f), 
        glm::vec3(1.0f,  0.0f,  0.0f), 
        glm::vec3(1.0f,  0.0f,  0.0f), 

        // back
        glm::vec3(0.0f,  0.0f, -1.0f), 
        glm::vec3(0.0f,  0.0f, -1.0f), 
        glm::vec3(0.0f,  0.0f, -1.0f), 
        glm::vec3(0.0f,  0.0f, -1.0f), 

        // right
        glm::vec3(-1.0f,  0.0f,  0.0f), 
        glm::vec3(-1.0f,  0.0f,  0.0f), 
        glm::vec3(-1.0f,  0.0f,  0.0f), 
        glm::vec3(-1.0f,  0.0f,  0.0f), 

        // top
        glm::vec3(0.0f,  1.0f,  0.0f), 
        glm::vec3(0.0f,  1.0f,  0.0f), 
        glm::vec3(0.0f,  1.0f,  0.0f), 
        glm::vec3(0.0f,  1.0f,  0.0f), 

        // bottom
        glm::vec3(0.0f, -1.0f,  0.0f), 
        glm::vec3(0.0f, -1.0f,  0.0f), 
        glm::vec3(0.0f, -1.0f,  0.0f), 
        glm::vec3(0.0f, -1.0f,  0.0f), 
    };

    TextureCoords& grassSideCoords = g_Texture2DManager.mainTextureMap->GetTextureCoords(1, 0);
    TextureCoords& grassTopCoords = g_Texture2DManager.mainTextureMap->GetTextureCoords(0, 0);
    TextureCoords& dirtCoords = g_Texture2DManager.mainTextureMap->GetTextureCoords(2, 0);

    std::vector<glm::vec2> uvs = std::vector<glm::vec2> {
        // top
        glm::vec2(grassSideCoords.tl.x, grassSideCoords.tl.y),
        glm::vec2(grassSideCoords.tr.x, grassSideCoords.tr.y),
        glm::vec2(grassSideCoords.br.x, grassSideCoords.br.y),
        glm::vec2(grassSideCoords.bl.x, grassSideCoords.bl.y),

        // right
        glm::vec2(grassSideCoords.tl.x, grassSideCoords.tl.y),
        glm::vec2(grassSideCoords.tr.x, grassSideCoords.tr.y),
        glm::vec2(grassSideCoords.br.x, grassSideCoords.br.y),
        glm::vec2(grassSideCoords.bl.x, grassSideCoords.bl.y),

        // back
        glm::vec2(grassSideCoords.tl.x, grassSideCoords.tl.y),
        glm::vec2(grassSideCoords.tr.x, grassSideCoords.tr.y),
        glm::vec2(grassSideCoords.br.x, grassSideCoords.br.y),
        glm::vec2(grassSideCoords.bl.x, grassSideCoords.bl.y),

        // left
        glm::vec2(grassSideCoords.tl.x, grassSideCoords.tl.y),
        glm::vec2(grassSideCoords.tr.x, grassSideCoords.tr.y),
        glm::vec2(grassSideCoords.br.x, grassSideCoords.br.y),
        glm::vec2(grassSideCoords.bl.x, grassSideCoords.bl.y),

        // top
        glm::vec2(grassTopCoords.tl.x, grassTopCoords.tl.y),
        glm::vec2(grassTopCoords.tr.x, grassTopCoords.tr.y),
        glm::vec2(grassTopCoords.br.x, grassTopCoords.br.y),
        glm::vec2(grassTopCoords.bl.x, grassTopCoords.bl.y),

        // bottom
        glm::vec2(dirtCoords.tl.x, dirtCoords.tl.y),
        glm::vec2(dirtCoords.tr.x, dirtCoords.tr.y),
        glm::vec2(dirtCoords.br.x, dirtCoords.br.y),
        glm::vec2(dirtCoords.bl.x, dirtCoords.bl.y),
    };

    std::vector<GLuint> indices = std::vector<GLuint> {
        // front
        0, 1, 2,
        0, 2, 3,

        // right
        4, 5, 6,
        4, 6, 7,

        // back
        8, 9, 10,
        8, 10, 11,

        // left
        12, 13, 14,
        12, 14, 15,

        // top
        16, 17, 18,
        16, 18, 19,

        // back
        20, 21, 22,
        20, 22, 23
    };

    m_numVertices = indices.size();

    m_VAO = std::make_unique<VertexArray>(
        &verticesPos,
        &indices,
        &normals,
        &colors,
        &uvs
    );
}

void Cube::virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material)
{
    MeshData meshData;
    meshData.VAO_id = m_VAO->getID();
    meshData.num_vertices = m_numVertices;
    meshData.material = std::make_shared<Material>(g_ShaderManager.basicShader);

    meshData.material->setAlbedoMap(*g_Texture2DManager.mainTextureMap);
    meshData.material->setSpecularMap(*g_Texture2DManager.mainTextureSpecularMap);

    g_ECSManager->AddComponent<MeshData>(entityUID, meshData);
}

HNCRSP_NAMESPACE_END