#include "DefaultScene.h"

#include "src/managers/Texture2DManager.h"
#include "src/managers/SceneManager.h"


using namespace Honeycrisp;

DefaultScene::DefaultScene()
{
    m_shader = CreateShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/BlinnPhongTint.frag")
    );
    m_adjustableColorShader = CreateShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/AdjustableColor.frag")
    );
    m_pointLight = CreateLight<PointLight>(
        glm::vec3(10.0f, 2.0f, 10.0f),  // pos
        glm::vec3(1.0f, 1.0f, 1.0f),    // color
        0.1f, 0.5f, 1.0f                // ambient - diffuse - specular
    );

    m_cube = CreateRenderObj<Cube>();
    m_cube->SetShader(m_shader);
    m_cube->SetTransform(Transform(glm::vec3(-1.0f, -3.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

    m_billboard = CreateRenderObj<Billboard>(3.0f, 1.0f, g_Texture2DManager.mainTextureMap);
    m_billboard->SetPosition(glm::vec3(0.0f, 4.0f, 3.0f));

    std::vector<glm::vec3> vertices = {
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

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    #define RAND dist(mt)

    std::vector<glm::vec3> colors = {
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),

        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),

        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),

        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),

        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),

        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
        glm::vec3(RAND, RAND, RAND),
    };

    std::vector<glm::vec3> normals = {
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

    std::vector<GLuint> indices = {
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

    m_customMesh = CreateRenderObj<Mesh>(
        &vertices,
        &indices,
        &normals,
        &colors,
        nullptr
    );
    m_customMesh->SetTransform(Transform(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    ));
    m_customMesh->SetShader(m_adjustableColorShader);

    SetInitialUniforms();
}

void DefaultScene::OnUpdate(const float& dt)
{
    m_adjustableColorShader->SetVec3Unf("u_color", glm::vec3(m_color));
}

void DefaultScene::SetInitialUniforms(void)
{
    DrawData& meshData = g_ECSManager.GetComponent<DrawData>(m_customMesh->entityUID);
    meshData.materials[0]->SetAlbedoMap(g_Texture2DManager.mainTextureMap);
    meshData.materials[0]->SetSpecularMap(g_Texture2DManager.mainTextureSpecularMap);
}

void DefaultScene::OnImGui(void)
{
    ImGui::Text("Some text in DefaultScene.cpp");

    ImGui::SliderFloat("Color", &m_color, 0.0f, 1.0f);
}