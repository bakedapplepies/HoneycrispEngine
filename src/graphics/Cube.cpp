#include "Cube.h"
#include "src/managers/Texture2DManager.h"
#include "src/managers/ShaderManager.h"
#include "src/managers/SceneManager.h"


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

    TextureAtlas& grassAtlas = g_Texture2DManager.GetAtlas(3, 1);
    QuadUV& grassSideUVs = grassAtlas.GetQuadUVs(1, 0);
    QuadUV& grassTopUVs = grassAtlas.GetQuadUVs(0, 0);
    QuadUV& dirtUVs = grassAtlas.GetQuadUVs(2, 0);

    std::vector<glm::vec2> uvs = std::vector<glm::vec2> {
        // top
        grassSideUVs.tl,
        grassSideUVs.tr,
        grassSideUVs.br,
        grassSideUVs.bl,

        // right
        grassSideUVs.tl,
        grassSideUVs.tr,
        grassSideUVs.br,
        grassSideUVs.bl,

        // back
        grassSideUVs.tl,
        grassSideUVs.tr,
        grassSideUVs.br,
        grassSideUVs.bl,

        // left
        grassSideUVs.tl,
        grassSideUVs.tr,
        grassSideUVs.br,
        grassSideUVs.bl,

        // top
        grassTopUVs.tl,
        grassTopUVs.tr,
        grassTopUVs.br,
        grassTopUVs.bl,

        // bottom
        dirtUVs.tl,
        dirtUVs.tr,
        dirtUVs.br,
        dirtUVs.bl,
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

void Cube::virt_AddDrawDataToRenderer(EntityUID entityUID)
{
    // std::shared_ptr<Shader> cubeShader = g_ShaderManager.GetShader(
    //     FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
    //     FileSystem::Path("resources/shaders/BlinnPhongTintFragment.glsl")
    // );

    DrawData drawData;
    drawData.VAO_id = m_VAO->getID();
    drawData.meta_data.emplace_back(0, m_numVertices);
    drawData.materials.push_back(std::make_shared<Material>(g_ShaderManager.basicShader));

    drawData.materials[0]->setAlbedoMap(g_Texture2DManager.mainTextureMap);
    drawData.materials[0]->setSpecularMap(g_Texture2DManager.mainTextureSpecularMap);

    g_ECSManager->AddComponent<DrawData>(entityUID, drawData);
}

HNCRSP_NAMESPACE_END