#include "DefaultScene.h"

#include "src/core/Texture2DManager.h"


using namespace Honeycrisp;

DefaultScene::DefaultScene()
{
    shader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/BlinnPhongFragment.glsl")
    );

    cube = CreateStaticRenderObj<Cube>();
    cube->setShader(shader);
    cube->setTransform(Transform(glm::vec3(-1.0f, -3.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

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

    customMesh = CreateStaticRenderObj<Mesh>(
        &vertices,
        &indices,
        &normals,
        &colors,
        nullptr
    );
    customMesh->setTransform(Transform(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    ));

    SetInitialUniforms();
}

void DefaultScene::OnUpdate(const float& dt)
{
}

void DefaultScene::SetInitialUniforms(void)
{
    MeshData& meshData = g_ECSManager->GetComponent<MeshData>(customMesh->entityUID);
    meshData.material->setAlbedoMap(g_Texture2DManager.mainTextureMap);
    meshData.material->setSpecularMap(g_Texture2DManager.mainTextureSpecularMap);

    // lighting
    shader->setIntUnf("u_material.albedo", g_Texture2DManager.mainTextureMap->getTextureUnit());
    shader->setIntUnf("u_material.specular", g_Texture2DManager.mainTextureSpecularMap->getTextureUnit());
    shader->setFloatUnf("u_material.shininess", 32.0f);

    // dir light
    shader->setVec3Unf("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    shader->setVec3Unf("u_dirLight.ambient", glm::vec3(1, 1, 1) * 0.1f);
    shader->setVec3Unf("u_dirLight.diffuse", glm::vec3(1, 1, 1) * 0.7f);
    shader->setVec3Unf("u_dirLight.specular", glm::vec3(1, 1, 1));

    // point light
    shader->setVec3Unf("u_pointLight.position", glm::vec3(10, 2, 10));
    shader->setVec3Unf("u_pointLight.ambient", 0.1f * glm::vec3(1.0));
    shader->setVec3Unf("u_pointLight.diffuse", 0.5f * glm::vec3(1.0));
    shader->setVec3Unf("u_pointLight.specular", 1.0f * glm::vec3(1.0));
    shader->setFloatUnf("u_pointLight.constant", 1.0f);
    shader->setFloatUnf("u_pointLight.linear", 0.001f);
    shader->setFloatUnf("u_pointLight.quadratic", 0.0002f);

    // spot light
    shader->setFloatUnf("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    shader->setFloatUnf("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    shader->setVec3Unf("u_spotLight.ambient", 0.1f * glm::vec3(1.0));
    shader->setVec3Unf("u_spotLight.diffuse", 0.5f * glm::vec3(1.0));
    shader->setVec3Unf("u_spotLight.specular", 1.0f * glm::vec3(1.0));
    shader->setFloatUnf("u_spotLight.constant", 1.0f);
    shader->setFloatUnf("u_spotLight.linear", 0.07f);
    shader->setFloatUnf("u_spotLight.quadratic", 0.0045f);
}

void DefaultScene::OnImGui(void)
{
    ImGui::Text("Some text in DefaultScene.cpp");
}