#include "DefaultSceneTwo.h"
#include "src/scenes/DefaultSceneTwo.h"


using namespace Honeycrisp;

DefaultSceneTwo::DefaultSceneTwo()
{
    pointLight = std::make_unique<PointLight>(
        glm::vec3(0.0f, 0.0f, 0.0f),  // pos
        glm::vec3(1.0f, 1.0f, 1.0f),  // color
        0.1f, 0.5f, 1.0f              // ambient - diffuse - specular
    );

    InitializeShaders();

    cube = CreateStaticRenderObj<Cube>();
    cubeTransform = &g_ECSManager->GetComponent<Transform>(cube->entityUID);
    cube->setShader(phongWTintShader);
    cube->setTransform(Transform(glm::vec3(1.0f, 10.0f ,5.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

    TextureAtlas& grassAtlas = g_Texture2DManager.GetAtlas(3, 1);
    QuadUV& grassUV = grassAtlas.GetQuadUVs(0, 0);
    unsigned int width = 50, height = 50;
    unsigned int vertW = width+1, vertH = height+1;
    unsigned int totalVerts = vertW * vertH;
    glm::vec2 uvDistVertical = grassUV.tl - grassUV.bl;
    glm::vec2 uvDistHorizontal = grassUV.br - grassUV.bl;

    std::vector<glm::vec3> vertices;
    vertices.reserve(totalVerts);
    for (unsigned int i = 0; i < vertH; i++)
    {
        for (unsigned int j = 0; j < vertW; j++)
        {
            vertices.emplace_back(glm::vec3((float)i, 0.0f, (float)j));
        }
    }

    std::vector<glm::vec3> colors;
    colors.reserve(totalVerts);
    for (unsigned int i = 0; i < totalVerts; i++)
    {
        colors.emplace_back(glm::vec3(0.369f, 0.616f, 0.204f));
    }

    std::vector<glm::vec3> normals;
    normals.reserve(totalVerts);
    for (unsigned int i = 0; i < totalVerts; i++)
    {
        normals.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    std::vector<glm::vec2> uvs;
    uvs.reserve(totalVerts);
    for (unsigned int i = 0; i < vertW; i++)
    {
        for (unsigned int j = 0; j < vertH; j++)
        {
            uvs.emplace_back(grassUV.bl + 
            uvDistHorizontal * ((float)i/(float)(vertW-1)) +
            uvDistVertical * ((float)j/(float)(vertH-1)));
        }
    }

    std::vector<GLuint> indices;
    indices.reserve(width*height*6);
    for (GLuint h = 0; h < height; h++)
    {
        for (GLuint w = 0; w < width; w++)
        {
            indices.emplace_back(h*vertW + w);
            indices.emplace_back((h+1)*vertW + w);
            indices.emplace_back(h*vertW + w+1);

            indices.emplace_back((h+1)*vertW + w+1);
            indices.emplace_back(h*vertW + w+1);
            indices.emplace_back((h+1)*vertW + w);
        }
    }

    mesh = CreateStaticRenderObj<Mesh>(
        &vertices,
        &indices,
        &normals,
        &colors,
        &uvs
    );
    mesh->setTransform(Transform(glm::vec3(-8.0f, -6.0f, -20.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    mesh->setShader(phongWTintShader);
    std::shared_ptr<Material> meshMaterial = g_ECSManager->GetComponent<DrawData>(mesh->entityUID).materials[0];
    meshMaterial->setAlbedoMap(g_Texture2DManager.mainTextureMap);
    meshMaterial->setSpecularMap(g_Texture2DManager.mainTextureSpecularMap);
    meshMaterial->setShininess(128.0f);

    backpackModel = CreateStaticRenderObj<Model>(
        FileSystem::Path("resources/models/backpack/backpack.obj"),
        phongShader,
        true  // flip uv
    );
    backpackModel->setTransform(Transform(glm::vec3(2.0f, 2.0f, -18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f)));
    // Material* backpackMaterial = backpackModel->getMaterial();
    // backpackMaterial->setShininess(64.0f);

    // Author: Eydeet (https://skfb.ly/ouB6N)
    // appleModel = CreateStaticRenderObj<Model>(FileSystem::Path("resources/models/apple/source/apple.fbx"), phongShader, false);
    // appleModel->setTransform(Transform(glm::vec3(10.0f, 2.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f)));
    // Material* appleMaterial = appleModel->getMaterial();
    // appleMaterial->setShininess(32);

    // appleModelNormal = CreateStaticRenderObj<Model>(FileSystem::Path("resources/models/apple/source/apple.fbx"), normalShader, false);
    // appleModelNormal->setTransform(Transform(glm::vec3(10.0f, 2.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f)));
    // appleModelNormal->getMaterial()->setShininess(32);

    sponza = CreateStaticRenderObj<Honeycrisp::Model>(
        FileSystem::Path("resources/models/sponza/sponza.obj"),
        phongShader,
        false
    );
    Transform& sponzaTransform = g_ECSManager->GetComponent<Transform>(sponza->entityUID);
    sponzaTransform.position = glm::vec3(0.0f, 2.0f, -2.0f);
    sponzaTransform.scale = glm::vec3(0.008f);

    CreateCubemap(
        FileSystem::Path("resources/textures/cubemaps/skybox/right.jpg"),
        FileSystem::Path("resources/textures/cubemaps/skybox/left.jpg"),
        FileSystem::Path("resources/textures/cubemaps/skybox/top.jpg"),
        FileSystem::Path("resources/textures/cubemaps/skybox/bottom.jpg"),
        FileSystem::Path("resources/textures/cubemaps/skybox/front.jpg"),
        FileSystem::Path("resources/textures/cubemaps/skybox/back.jpg")
    );

    SetInitialUniforms();
}

void DefaultSceneTwo::OnUpdate(const float& dt)
{
    phongShader->setVec3Unf("u_pointLight.position", pointLight->position);
    phongWTintShader->setVec3Unf("u_pointLight.position", pointLight->position);
    normalShader->setFloatUnf("u_normal_length", m_u_normal_length);

    cubeTransform->eulerAngles += glm::vec3(1.0f, 1.0f, 0.0f) * dt;
    // cubeTransform->position = glm::vec3(0.0f, sinf(glfwGetTime()) * 7.0f, 0.0f);
}

void DefaultSceneTwo::InitializeShaders(void)
{
    phongShader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/PureAlbedoFragment.glsl")
    );
    phongWTintShader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/BlinnPhongTintFragment.glsl")
        // FileSystem::Path("resources/shaders/WaveGeometry.glsl")  // TODO: just to calculate normals, maybe rename file to CalcNormGeometry.glsl
    );
    normalShader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/NormalVertex.glsl"),
        FileSystem::Path("resources/shaders/YellowFragment.glsl"),
        FileSystem::Path("resources/shaders/NormalGeometry.glsl")
    );
}

void DefaultSceneTwo::SetInitialUniforms(void)
{
    // dir light
    phongShader->setVec3Unf("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    phongShader->setVec3Unf("u_dirLight.ambient", glm::vec3(0.1f));
    phongShader->setVec3Unf("u_dirLight.diffuse", glm::vec3(0.7f));
    phongShader->setVec3Unf("u_dirLight.specular", glm::vec3(1.0f));

    // point light
    phongShader->setVec3Unf("u_pointLight.position", pointLight->position);
    phongShader->setVec3Unf("u_pointLight.ambient", pointLight->getAmbient());
    phongShader->setVec3Unf("u_pointLight.diffuse", pointLight->getDiffuse());
    phongShader->setVec3Unf("u_pointLight.specular", pointLight->getSpecular());
    phongShader->setFloatUnf("u_pointLight.constant", pointLight->attenuation_constant);
    phongShader->setFloatUnf("u_pointLight.linear", pointLight->attenuation_linear);
    phongShader->setFloatUnf("u_pointLight.quadratic", pointLight->attenuation_quadratic);

    // spot light
    phongShader->setFloatUnf("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    phongShader->setFloatUnf("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    phongShader->setVec3Unf("u_spotLight.ambient", glm::vec3(0.1f));
    phongShader->setVec3Unf("u_spotLight.diffuse", glm::vec3(0.5f));
    phongShader->setVec3Unf("u_spotLight.specular", glm::vec3(1.0f));
    phongShader->setFloatUnf("u_spotLight.constant", 1.0f);
    phongShader->setFloatUnf("u_spotLight.linear", 0.07f);
    phongShader->setFloatUnf("u_spotLight.quadratic", 0.0045f);
    
    // dir light
    phongWTintShader->setVec3Unf("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    phongWTintShader->setVec3Unf("u_dirLight.ambient", glm::vec3(0.1f));
    phongWTintShader->setVec3Unf("u_dirLight.diffuse", glm::vec3(0.7f));
    phongWTintShader->setVec3Unf("u_dirLight.specular", glm::vec3(1.0f));

    // point light
    phongWTintShader->setVec3Unf("u_pointLight.position", pointLight->position);
    phongWTintShader->setVec3Unf("u_pointLight.ambient", pointLight->getAmbient());
    phongWTintShader->setVec3Unf("u_pointLight.diffuse", pointLight->getDiffuse());
    phongWTintShader->setVec3Unf("u_pointLight.specular", pointLight->getSpecular());
    phongWTintShader->setFloatUnf("u_pointLight.constant", pointLight->attenuation_constant);
    phongWTintShader->setFloatUnf("u_pointLight.linear", pointLight->attenuation_linear);
    phongWTintShader->setFloatUnf("u_pointLight.quadratic", pointLight->attenuation_quadratic);

    // spot light
    phongWTintShader->setFloatUnf("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    phongWTintShader->setFloatUnf("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    phongWTintShader->setVec3Unf("u_spotLight.ambient", glm::vec3(0.1f));
    phongWTintShader->setVec3Unf("u_spotLight.diffuse", glm::vec3(0.5f));
    phongWTintShader->setVec3Unf("u_spotLight.specular", glm::vec3(1.0f));
    phongWTintShader->setFloatUnf("u_spotLight.constant", 1.0f);
    phongWTintShader->setFloatUnf("u_spotLight.linear", 0.07f);
    phongWTintShader->setFloatUnf("u_spotLight.quadratic", 0.0045f);
}

void DefaultSceneTwo::OnImGui(void)
{
    ImGui::Text("Point light");
    ImGui::SliderFloat3("position", glm::value_ptr(pointLight->position), -100.0f, 100.0f);

    ImGui::SliderFloat("u_normal_length", &m_u_normal_length, 0.001f, 1.0f);
}