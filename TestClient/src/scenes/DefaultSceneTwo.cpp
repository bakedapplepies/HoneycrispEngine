#include "DefaultSceneTwo.h"
#include "api/core/Utils.h"
#include "api/systems/Renderer.h"
#include "api/graphics/TextureAtlas.h"
#include "api/managers/Texture2DManager.h"
#include "api/managers/ShaderManager.h"

using namespace Honeycrisp;

DefaultSceneTwo::DefaultSceneTwo(const char* scene_name)
    : Scene(scene_name)
{
    const Camera* camera = g_ECSManager.GetSystem<Renderer>()->GetCamera();
    pointLight = CreateLight<PointLight>(
        glm::vec3(0.0f, 0.0f, 0.0f),  // pos
        glm::vec3(1.0f, 1.0f, 1.0f),  // color
        0.3f, 0.5f, 1.0f              // ambient - diffuse - specular
    );
    // dirLight = CreateLight<DirectionalLight>(
    //     glm::vec3(0.0f, -1.0f, 0.0),
    //     glm::vec3(1.0f, 1.0f, 1.0f),
    //     0.05f, 0.5f, 1.0f
    // );

    InitializeShaders();
    _CreateCube();
    _CreatePlane();
    _CreateModels();
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
    // normalShader->setFloatUnf("u_normal_length", m_u_normal_length);

    cubeTransform->eulerAngles += glm::vec3(1.0f, 1.0f, 0.0f) * dt;
    pointLight->position = cubeTransform->position;
    // cubeTransform->position = glm::vec3(0.0f, sinf(glfwGetTime()) * 7.0f, 0.0f);
}

void DefaultSceneTwo::InitializeShaders(void)
{
    phongShader = CreateShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/BlinnPhong.frag")
    );
    phongWTintShader = CreateShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/BlinnPhongTint.frag")
        // FileSystem::Path("resources/shaders/Wave.geom")  // TODO: just to calculate normals, maybe rename file to CalcNorm.geom
    );
    // normalShader = CreateShader(
    //     FileSystem::Path("resources/shaders/Normal.vert"),
    //     FileSystem::Path("resources/shaders/Yellow.frag"),
    //     FileSystem::Path("resources/shaders/Normal.geom")
    // );
}

void DefaultSceneTwo::SetInitialUniforms(void)
{}

void DefaultSceneTwo::OnImGui(void)
{
    static CallbackData* callback_data = GetCallbackData();
    static Renderer* renderer = g_ECSManager.GetSystem<Renderer>();

    // ImGui::Text("Point light #1");
    // if (   ImGui::SliderFloat("position.x", &pointLight->position.x, -50.0f, 50.0f)
    //     || ImGui::SliderFloat("position.y", &pointLight->position.y, -50.0f, 50.0f)
    //     || ImGui::SliderFloat("position.z", &pointLight->position.z, -50.0f, 50.0f)
    // ) {
    //     phongShader->SetVec3Unf("u_point_light.position", pointLight->position);
    //     phongWTintShader->SetVec3Unf("u_point_light.position", pointLight->position);
    // }

    // ImGui::Text("Directional light");
    // if (ImGui::SliderFloat("direction.x", &dirLight->direction.x, -1.0f, 1.0f)
    //  || ImGui::SliderFloat("direction.y", &dirLight->direction.y, -1.0f, 1.0f)
    //  || ImGui::SliderFloat("direction.z", &dirLight->direction.z, -1.0f, 1.0f)
    // ) {
    //     phongShader->SetVec3Unf("u_dir_light.direction", glm::normalize(dirLight->direction));
    //     phongWTintShader->SetVec3Unf("u_dir_light.direction", glm::normalize(dirLight->direction));

    //     renderer->directionalLightDir = glm::normalize(dirLight->direction);
    // }

    ImGui::Text("Cube position");
    if (ImGui::SliderFloat("cubePos.x", &cubeTransform->position.x, -10.0f, 10.0f)
     || ImGui::SliderFloat("cubePos.y", &cubeTransform->position.y, -10.0f, 10.0f)
     || ImGui::SliderFloat("cubePos.z", &cubeTransform->position.z, -10.0f, 10.0f))
    {
        UpdateLight(pointLight);
    }

    ImGui::SliderFloat("Depth Camera Resolution", &renderer->depthCameraResolution, 1.0f, 2.0f);

    ImGui::SliderFloat("u_normal_length", &m_u_normal_length, 0.001f, 1.0f);
}

void DefaultSceneTwo::_CreateCube(void)
{
    Material cubeMaterial(phongWTintShader);
    cubeMaterial.SetAlbedoMap(g_Texture2DManager.mainTextureMap);
    cubeMaterial.SetSpecularMap(g_Texture2DManager.mainTextureSpecularMap);
    cube = CreateRenderObj<Cube>(cubeMaterial);
    cubeTransform = &g_ECSManager.GetComponent<Transform>(cube->entityUID);
    cube->SetTransform(Transform(glm::vec3(1.0f, 10.0f ,5.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
}

void DefaultSceneTwo::_CreatePlane(void)
{
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
            vertices.emplace_back((float)i, 0.0f, (float)j);
        }
    }

    std::vector<glm::vec3> colors;
    colors.reserve(totalVerts);
    for (unsigned int i = 0; i < totalVerts; i++)
    {
        colors.emplace_back(0.369f, 0.616f, 0.204f);
    }

    std::vector<glm::vec3> normals;
    normals.reserve(totalVerts);
    for (unsigned int i = 0; i < totalVerts; i++)
    {
        normals.emplace_back(0.0f, 1.0f, 0.0f);
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

    Material meshMaterial(phongWTintShader);
    meshMaterial.SetAlbedoMap(g_Texture2DManager.mainTextureMap);
    meshMaterial.SetSpecularMap(g_Texture2DManager.mainTextureSpecularMap);
    meshMaterial.SetShininess(128.0f);
    mesh = CreateRenderObj<Mesh>(
        meshMaterial,
        &vertices,
        &indices,
        &normals,
        &colors,
        &uvs
    );
    const Texture2D* meshAlbedo = g_Texture2DManager.GetTexture2D(
        FileSystem::Path("resources/textures/cubemaps/skybox/back.jpg"),
        ETextureType::ALBEDO 
    );
    // mesh->SetMaterial(0, meshMaterial);
    
    mesh->SetTransform(Transform(glm::vec3(-8.0f, -6.0f, -20.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    mesh->SetShader(phongWTintShader);
}

void DefaultSceneTwo::_CreateModels(void)
{
    backpackModel = CreateModel<Model>(
        FileSystem::Path("resources/models/backpack/backpack.obj"),
        phongShader,
        true  // flip uv
    );
    backpackModel->SetTransform(Transform(glm::vec3(2.0f, 2.0f, -18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f)));

    // Author: Eydeet (https://skfb.ly/ouB6N)
    appleModel = CreateModel<Model>(
        FileSystem::Path("resources/models/apple/source/apple.fbx"),
        phongShader,
        false
    );
    appleModel->SetTransform(Transform(
        glm::vec3(10.0f, 2.0f, 17.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.3f)
    ));

    sponza = CreateModel<Model>(
        FileSystem::Path("resources/models/sponza2/Sponza.gltf"),
        phongShader,
        false
    );
    Transform& sponzaTransform = g_ECSManager.GetComponent<Transform>(sponza->entityUID);
    sponzaTransform.position = glm::vec3(0.0f, 2.0f, -2.0f);
    sponzaTransform.scale = glm::vec3(0.008f);
}