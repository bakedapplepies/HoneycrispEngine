#include "DefaultSceneTwo.h"
#include "src/scenes/DefaultSceneTwo.h"
#include "src/managers/SceneManager.h"


using namespace Honeycrisp;

DefaultSceneTwo::DefaultSceneTwo()
{
    const Camera& camera = g_SceneManager.GetCallbackData()->camera;
    pointLight = CreateLight<PointLight>(
        glm::vec3(0.0f, 0.0f, 0.0f),  // pos
        glm::vec3(1.0f, 1.0f, 1.0f),  // color
        0.3f, 0.5f, 1.0f              // ambient - diffuse - specular
    );
    spotLight = CreateLight<SpotLight>(
        camera.position,
        camera.direction,
        glm::vec3(1.0f, 1.0f, 1.0f),
        0.3f, 0.5f, 1.0f
    );

    InitializeShaders();

    // TODO: Parse shaders
    // const int maxLength = 512;
    // char name[maxLength];

    // int uniformCount;
    // glGetProgramiv(phongShader->GetID(), GL_ACTIVE_UNIFORMS, &uniformCount);
    // for (int i = 0; i < uniformCount; i++)
    // {
    //     GLint size;
    //     GLenum type;
    //     glGetActiveUniform(phongShader->GetID(), i, maxLength, NULL, &size, &type, name);

    //     HNCRSP_LOG_INFO(fmt::format("Size: {}, Type: {}\nInfo: {}", size, type, name));
    // }

    // int attribCount;
    // glGetProgramiv(phongShader->GetID(), GL_ACTIVE_ATTRIBUTES, &attribCount);
    // for (int i = 0; i < attribCount; i++)
    // {
    //     GLint size;
    //     GLenum type;
    //     glGetActiveAttrib(phongShader->GetID(), i, maxLength, NULL, &size, &type, name);

    //     HNCRSP_LOG_INFO(fmt::format("Size: {}, Type: {}\nInfo: {}", size, type, name));
    // }


    cube = CreateStaticRenderObj<Cube>();
    cubeTransform = &g_ECSManager.GetComponent<Transform>(cube->entityUID);
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

    mesh = CreateStaticRenderObj<Mesh>(
        &vertices,
        &indices,
        &normals,
        &colors,
        &uvs
    );
    mesh->setTransform(Transform(glm::vec3(-8.0f, -6.0f, -20.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    mesh->setShader(phongWTintShader);
    DrawData meshDrawDataCopy = g_ECSManager.GetComponent<DrawData>(mesh->entityUID);
    std::shared_ptr<Material> meshMaterial = meshDrawDataCopy.materials[0];
    meshMaterial->SetAlbedoMap(g_Texture2DManager.mainTextureMap);
    meshMaterial->SetSpecularMap(g_Texture2DManager.mainTextureSpecularMap);
    meshMaterial->SetShininess(128.0f);

    // TODO: Make it easier for when adding new textures to material, the Renderer would update itself
    g_ECSManager.RemoveComponent<DrawData>(mesh->entityUID);
    g_ECSManager.AddComponent<DrawData>(mesh->entityUID, meshDrawDataCopy);

    backpackModel = CreateStaticRenderObj<Model>(
        FileSystem::Path("resources/models/backpack/backpack.obj"),
        phongShader,
        true  // flip uv
    );
    backpackModel->setTransform(Transform(glm::vec3(2.0f, 2.0f, -18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f)));
    // Material* backpackMaterial = backpackModel->getMaterial();
    // backpackMaterial->setShininess(64.0f);

    // Author: Eydeet (https://skfb.ly/ouB6N)
    appleModel = CreateStaticRenderObj<Model>(FileSystem::Path("resources/models/apple/source/apple.fbx"), phongShader, false);
    appleModel->setTransform(Transform(
        glm::vec3(10.0f, 2.0f, 17.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.3f)
    ));
    // Material* appleMaterial = appleModel->getMaterial();
    // appleMaterial->setShininess(32);

    // appleModelNormal = CreateStaticRenderObj<Model>(FileSystem::Path("resources/models/apple/source/apple.fbx"), normalShader, false);
    // appleModelNormal->setTransform(Transform(glm::vec3(10.0f, 2.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f)));
    // appleModelNormal->getMaterial()->setShininess(32);

    sponza = CreateStaticRenderObj<Model>(
        FileSystem::Path("resources/models/sponza2/Sponza.gltf"),
        phongShader,
        false
    );
    // sponzaNormal = CreateStaticRenderObj<Honeycrisp::Model>(
    //     FileSystem::Path("resources/models/sponza/sponza.obj"),
    //     normalShader,
    //     false
    // );
    Transform& sponzaTransform = g_ECSManager.GetComponent<Transform>(sponza->entityUID);
    sponzaTransform.position = glm::vec3(0.0f, 2.0f, -2.0f);
    sponzaTransform.scale = glm::vec3(0.008f);
    // Transform& sponzaNormalTransform = g_ECSManager.GetComponent<Transform>(sponzaNormal->entityUID);
    // sponzaNormalTransform.position = glm::vec3(0.0f, 2.0f, -2.0f);
    // sponzaNormalTransform.scale = glm::vec3(0.008f);

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
    // cubeTransform->position = glm::vec3(0.0f, sinf(glfwGetTime()) * 7.0f, 0.0f);
}

void DefaultSceneTwo::InitializeShaders(void)
{
    phongShader = CreateShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/BlinnPhongFragment.glsl")
    );
    phongWTintShader = CreateShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/BlinnPhongTintFragment.glsl")
        // FileSystem::Path("resources/shaders/WaveGeometry.glsl")  // TODO: just to calculate normals, maybe rename file to CalcNormGeometry.glsl
    );
    // normalShader = CreateShader(
    //     FileSystem::Path("resources/shaders/NormalVertex.glsl"),
    //     FileSystem::Path("resources/shaders/YellowFragment.glsl"),
    //     FileSystem::Path("resources/shaders/NormalGeometry.glsl")
    // );
}

void DefaultSceneTwo::SetInitialUniforms(void)
{
}

void DefaultSceneTwo::OnImGui(void)
{
    static CallbackData* callback_data = g_SceneManager.GetMutableCallbackData();

    ImGui::Text("Point light");
    if (  ImGui::SliderFloat("position.x", &pointLight->position.x, -50.0f, 50.0f)
        | ImGui::SliderFloat("position.y", &pointLight->position.y, -50.0f, 50.0f)
        | ImGui::SliderFloat("position.z", &pointLight->position.z, -50.0f, 50.0f)
    ) {
        callback_data->dirLightPos = pointLight->position;
        phongShader->SetVec3Unf("u_point_light.position", pointLight->position);
        phongWTintShader->SetVec3Unf("u_point_light.position", pointLight->position);
    }

    ImGui::SliderFloat("u_normal_length", &m_u_normal_length, 0.001f, 1.0f);
}