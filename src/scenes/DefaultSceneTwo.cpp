#include "DefaultSceneTwo.h"
#include "src/scenes/DefaultSceneTwo.h"


using namespace Honeycrisp;

DefaultSceneTwo::DefaultSceneTwo()
    : Scene()
{
    bgColor = glm::vec3(0.0f);
    pointLight = std::make_unique<PointLight>(
        glm::vec3(0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        0.1f, 0.5f, 1.0f
    );

    InitializeShaders();

    cube = CreateStaticRenderObj<Cube>();
    cube->setShader(shader);
    cube->setTransform(Transform(glm::vec3(1.0f, 3.0f ,5.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

    TextureCoords& grassUV = g_Texture2DManager.mainTextureMap->GetTextureCoords(0, 0);
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
    mesh->setShader(wackyShader);

    mesh->setTransform(Transform(glm::vec3(0.0f, -6.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

    model = CreateStaticRenderObj<Model>(FileSystem::Path("resources/models/backpack/backpack.obj"));
    model->setShader(backpackShader);
    model->setTransform(Transform(glm::vec3(10.0f, 2.0f, 7.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(5.0f)));

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

DefaultSceneTwo::~DefaultSceneTwo()
{
}

void DefaultSceneTwo::OnUpdate()
{
    shader->setVec3Unf("u_pointLight.position", pointLight->position);
    wackyShader->setVec3Unf("u_pointLight.position", pointLight->position);
    backpackShader->setVec3Unf("u_pointLight.position", pointLight->position);

    Transform& cubeTransform = g_ECSManager->GetComponent<Transform>(cube->entityUID);
    cubeTransform.eulerAngles += glm::vec3(0.01f, 0.02f, 0.04f);
    DrawCubemap();
    // mesh->Draw(normalWaveShader.get());  // using another shader to render normals
}

void DefaultSceneTwo::InitializeShaders(void)
{
    shader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/PhongShadingFragment.glsl")
    );
    backpackShader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/PhongShadingFragment.glsl")  // update cubemap texture unit uniform
    );
    wackyShader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/PhongShadingFragment.glsl")
        // FileSystem::Path("resources/shaders/WaveGeometry.glsl")  // TODO: just to calculate normals, maybe rename file to CalcNormGeometry.glsl
    );
    normalWaveShader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/WaveNormalVertex.glsl"),
        FileSystem::Path("resources/shaders/YellowFragment.glsl"),
        FileSystem::Path("resources/shaders/NormalGeometry.glsl")
    );
}

void DefaultSceneTwo::SetInitialUniforms(void)
{
    // lighting
    shader->setIntUnf("u_material.albedo", g_Texture2DManager.mainTextureMap->getTextureUnit());
    shader->setIntUnf("u_material.specular", g_Texture2DManager.mainTextureSpecularMap->getTextureUnit());
    shader->setFloatUnf("u_material.shininess", 128.0f);

    // dir light
    shader->setVec3Unf("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    shader->setVec3Unf("u_dirLight.ambient", glm::vec3(0.1f));
    shader->setVec3Unf("u_dirLight.diffuse", glm::vec3(0.7f));
    shader->setVec3Unf("u_dirLight.specular", glm::vec3(1.0f));

    // point light
    shader->setVec3Unf("u_pointLight.position", pointLight->position);
    shader->setVec3Unf("u_pointLight.ambient", pointLight->getAmbient());
    shader->setVec3Unf("u_pointLight.diffuse", pointLight->getDiffuse());
    shader->setVec3Unf("u_pointLight.specular", pointLight->getSpecular());
    shader->setFloatUnf("u_pointLight.constant", pointLight->attenuation_constant);
    shader->setFloatUnf("u_pointLight.linear", pointLight->attenuation_linear);
    shader->setFloatUnf("u_pointLight.quadratic", pointLight->attenuation_quadratic);

    // spot light
    shader->setFloatUnf("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    shader->setFloatUnf("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    shader->setVec3Unf("u_spotLight.ambient", glm::vec3(0.1f));
    shader->setVec3Unf("u_spotLight.diffuse", glm::vec3(0.5f));
    shader->setVec3Unf("u_spotLight.specular", glm::vec3(1.0f));
    shader->setFloatUnf("u_spotLight.constant", 1.0f);
    shader->setFloatUnf("u_spotLight.linear", 0.07f);
    shader->setFloatUnf("u_spotLight.quadratic", 0.0045f);
    
    // lighting
    wackyShader->setIntUnf("u_material.albedo", g_Texture2DManager.mainTextureMap->getTextureUnit());
    wackyShader->setIntUnf("u_material.specular", g_Texture2DManager.mainTextureSpecularMap->getTextureUnit());
    wackyShader->setFloatUnf("u_material.shininess", 128.0f);

    // dir light
    wackyShader->setVec3Unf("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    wackyShader->setVec3Unf("u_dirLight.ambient", glm::vec3(0.1f));
    wackyShader->setVec3Unf("u_dirLight.diffuse", glm::vec3(0.7f));
    wackyShader->setVec3Unf("u_dirLight.specular", glm::vec3(1.0f));

    // point light
    wackyShader->setVec3Unf("u_pointLight.position", pointLight->position);
    wackyShader->setVec3Unf("u_pointLight.ambient", pointLight->getAmbient());
    wackyShader->setVec3Unf("u_pointLight.diffuse", pointLight->getDiffuse());
    wackyShader->setVec3Unf("u_pointLight.specular", pointLight->getSpecular());
    wackyShader->setFloatUnf("u_pointLight.constant", pointLight->attenuation_constant);
    wackyShader->setFloatUnf("u_pointLight.linear", pointLight->attenuation_linear);
    wackyShader->setFloatUnf("u_pointLight.quadratic", pointLight->attenuation_quadratic);

    // spot light
    wackyShader->setFloatUnf("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    wackyShader->setFloatUnf("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    wackyShader->setVec3Unf("u_spotLight.ambient", glm::vec3(0.1f));
    wackyShader->setVec3Unf("u_spotLight.diffuse", glm::vec3(0.5f));
    wackyShader->setVec3Unf("u_spotLight.specular", glm::vec3(1.0f));
    wackyShader->setFloatUnf("u_spotLight.constant", 1.0f);
    wackyShader->setFloatUnf("u_spotLight.linear", 0.07f);
    wackyShader->setFloatUnf("u_spotLight.quadratic", 0.0045f);

    // backpackShader->setIntUnf("cubemap", 10);
    // lighting
    backpackShader->setFloatUnf("u_material.shininess", 128.0f);

    // dir light
    backpackShader->setVec3Unf("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    backpackShader->setVec3Unf("u_dirLight.ambient", glm::vec3(1.0f));
    backpackShader->setVec3Unf("u_dirLight.diffuse", glm::vec3(0.7f));
    backpackShader->setVec3Unf("u_dirLight.specular", glm::vec3(1.0f));

    // point light
    backpackShader->setVec3Unf("u_pointLight.position", pointLight->position);
    backpackShader->setVec3Unf("u_pointLight.ambient", pointLight->getAmbient());
    backpackShader->setVec3Unf("u_pointLight.diffuse", pointLight->getDiffuse());
    backpackShader->setVec3Unf("u_pointLight.specular", pointLight->getSpecular());
    backpackShader->setFloatUnf("u_pointLight.constant", pointLight->attenuation_constant);
    backpackShader->setFloatUnf("u_pointLight.linear", pointLight->attenuation_linear);
    backpackShader->setFloatUnf("u_pointLight.quadratic", pointLight->attenuation_quadratic);

    // spot light
    backpackShader->setFloatUnf("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    backpackShader->setFloatUnf("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    backpackShader->setVec3Unf("u_spotLight.ambient", glm::vec3(0.1f));
    backpackShader->setVec3Unf("u_spotLight.diffuse", glm::vec3(0.5f));
    backpackShader->setVec3Unf("u_spotLight.specular", glm::vec3(1.0f));
    backpackShader->setFloatUnf("u_spotLight.constant", 1.0f);
    backpackShader->setFloatUnf("u_spotLight.linear", 0.07f);
    backpackShader->setFloatUnf("u_spotLight.quadratic", 0.0045f);
}

void DefaultSceneTwo::OnImGui(void)
{
    ImGui::Text("Point light");
    ImGui::SliderFloat3("position", glm::value_ptr(pointLight->position), -100.0f, 100.0f);
}