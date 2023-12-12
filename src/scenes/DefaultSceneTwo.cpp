#include "DefaultSceneTwo.h"
#include "../utils/utils.h"


DefaultSceneTwo::DefaultSceneTwo()
{
    bgColor = glm::vec3(1.0f);

    InitializeShaders();

    cube = CreateObject<Cube, EObjectMovement::NONE>();
    cube->setShader(shader);
    cube->addTransform(Transform(glm::vec3(1.0f, 3.0f ,5.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    cube->addTransform(Transform(glm::vec3(3.0f, 2.0f ,1.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    cube->addTransform(Transform(glm::vec3(-5.0f, -2.0f ,3.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

    TextureCoords& grassUV = Textures::mainTextureMap.GetTextureCoords(0, 0);
    int width = 50, height = 50;
    int vertW = width+1, vertH = height+1;
    int totalVerts = vertW * vertH;
    glm::vec2 uvDistVertical = grassUV.tl - grassUV.bl;
    glm::vec2 uvDistHorizontal = grassUV.br - grassUV.bl;

    std::vector<glm::vec3> vertices;
    vertices.reserve(totalVerts);
    for (int i = 0; i < vertH; i++)
    {
        for (int j = 0; j < vertW; j++)
        {
            vertices.emplace_back(glm::vec3((float)i, 0.0f, (float)j));
        }
    }

    std::vector<glm::vec3> colors;
    colors.reserve(totalVerts);
    for (int i = 0; i < totalVerts; i++)
    {
        colors.emplace_back(glm::vec3(0.369f, 0.616f, 0.204f));
    }

    std::vector<glm::vec2> uvs;
    uvs.reserve(totalVerts);
    for (int i = 0; i < vertW; i++)
    {
        for (int j = 0; j < vertH; j++)
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

    mesh = CreateObject<Mesh, EObjectMovement::NONE>();
    mesh->setShader(wackyShader);
    mesh->vertices = vertices;
    mesh->colors = colors;
    mesh->uvs = uvs;
    mesh->indices = indices;

    mesh->ConstructMesh();
    mesh->addTransform(Transform(glm::vec3(0.0f, -6.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

    model = CreateObject<Model, EObjectMovement::NONE>(FileSystem::Path("resources/models/backpack/backpack.obj"));
    model->setShader(backpackShader);
    model->addTransform(Transform(glm::vec3(10.0f, 2.0f, 7.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(5.0f)));

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

void DefaultSceneTwo::onUpdate()
{
    Draw();
    mesh->Draw(normalWaveShader);
}

void DefaultSceneTwo::InitializeShaders(void)
{
    shader = std::make_shared<Shader>(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/PhongShadingFragment.glsl")
    );
    backpackShader = std::make_shared<Shader>(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/PhongShadingFragment.glsl")  // update cubemap texture unit uniform
    );
    wackyShader = std::make_shared<Shader>(
        FileSystem::Path("resources/shaders/WaveVertex.glsl"),
        FileSystem::Path("resources/shaders/PhongShadingFragment.glsl"),
        FileSystem::Path("resources/shaders/WaveGeometry.glsl")
    );
    normalWaveShader = std::make_shared<Shader>(
        FileSystem::Path("resources/shaders/WaveNormalVertex.glsl"),
        FileSystem::Path("resources/shaders/YellowFragment.glsl"),
        FileSystem::Path("resources/shaders/NormalGeometry.glsl")
    );
}

void DefaultSceneTwo::SetInitialUniforms(void)
{
    // lighting
    shader->setIntUniform("u_material.albedo", Textures::mainTextureMap.getTextureUnit());
    shader->setIntUniform("u_material.specular", Textures::mainTextureSpecularMap.getTextureUnit());
    shader->setFloatUniform("u_material.shininess", 32.0f);

    // dir light
    shader->setVector3Uniform("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    shader->setVector3Uniform("u_dirLight.ambient", glm::vec3(1, 1, 1) * 0.1f);
    shader->setVector3Uniform("u_dirLight.diffuse", glm::vec3(1, 1, 1) * 0.7f);
    shader->setVector3Uniform("u_dirLight.specular", glm::vec3(1, 1, 1));

    // point light
    shader->setVector3Uniform("u_pointLight.position", glm::vec3(10, 2, 50));
    shader->setVector3Uniform("u_pointLight.ambient", 0.1f * glm::vec3(1.0));
    shader->setVector3Uniform("u_pointLight.diffuse", 0.5f * glm::vec3(1.0));
    shader->setVector3Uniform("u_pointLight.specular", 1.0f * glm::vec3(1.0));
    shader->setFloatUniform("u_pointLight.constant", 1.0f);
    shader->setFloatUniform("u_pointLight.linear", 0.001f);
    shader->setFloatUniform("u_pointLight.quadratic", 0.0002f);

    // spot light
    shader->setFloatUniform("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    shader->setFloatUniform("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    shader->setVector3Uniform("u_spotLight.ambient", 0.1f * glm::vec3(1.0));
    shader->setVector3Uniform("u_spotLight.diffuse", 0.5f * glm::vec3(1.0));
    shader->setVector3Uniform("u_spotLight.specular", 1.0f * glm::vec3(1.0));
    shader->setFloatUniform("u_spotLight.constant", 1.0f);
    shader->setFloatUniform("u_spotLight.linear", 0.07f);
    shader->setFloatUniform("u_spotLight.quadratic", 0.0045f);
    
    // lighting
    wackyShader->setIntUniform("u_material.albedo", Textures::mainTextureMap.getTextureUnit());
    wackyShader->setIntUniform("u_material.specular", Textures::mainTextureSpecularMap.getTextureUnit());
    wackyShader->setFloatUniform("u_material.shininess", 32.0f);

    // dir light
    wackyShader->setVector3Uniform("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    wackyShader->setVector3Uniform("u_dirLight.ambient", glm::vec3(1, 1, 1) * 0.1f);
    wackyShader->setVector3Uniform("u_dirLight.diffuse", glm::vec3(1, 1, 1) * 0.7f);
    wackyShader->setVector3Uniform("u_dirLight.specular", glm::vec3(1, 1, 1));

    // point light
    wackyShader->setVector3Uniform("u_pointLight.position", glm::vec3(10, 2, 10));
    wackyShader->setVector3Uniform("u_pointLight.ambient", 0.1f * glm::vec3(1.0));
    wackyShader->setVector3Uniform("u_pointLight.diffuse", 0.5f * glm::vec3(1.0));
    wackyShader->setVector3Uniform("u_pointLight.specular", 1.0f * glm::vec3(1.0));
    wackyShader->setFloatUniform("u_pointLight.constant", 1.0f);
    wackyShader->setFloatUniform("u_pointLight.linear", 0.001f);
    wackyShader->setFloatUniform("u_pointLight.quadratic", 0.0002f);

    // spot light
    wackyShader->setFloatUniform("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    wackyShader->setFloatUniform("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    wackyShader->setVector3Uniform("u_spotLight.ambient", 0.1f * glm::vec3(1.0));
    wackyShader->setVector3Uniform("u_spotLight.diffuse", 0.5f * glm::vec3(1.0));
    wackyShader->setVector3Uniform("u_spotLight.specular", 1.0f * glm::vec3(1.0));
    wackyShader->setFloatUniform("u_spotLight.constant", 1.0f);
    wackyShader->setFloatUniform("u_spotLight.linear", 0.07f);
    wackyShader->setFloatUniform("u_spotLight.quadratic", 0.0045f);

    // backpackShader->setIntUniform("cubemap", 10);
    // lighting
    backpackShader->setFloatUniform("u_material.shininess", 32.0f);

    // dir light
    backpackShader->setVector3Uniform("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    backpackShader->setVector3Uniform("u_dirLight.ambient", glm::vec3(1, 1, 1) * 0.1f);
    backpackShader->setVector3Uniform("u_dirLight.diffuse", glm::vec3(1, 1, 1) * 0.7f);
    backpackShader->setVector3Uniform("u_dirLight.specular", glm::vec3(1, 1, 1));

    // point light
    backpackShader->setVector3Uniform("u_pointLight.position", glm::vec3(15, 4, 15));
    backpackShader->setVector3Uniform("u_pointLight.ambient", 0.1f * glm::vec3(1.0));
    backpackShader->setVector3Uniform("u_pointLight.diffuse", 0.6f * glm::vec3(1.0));
    backpackShader->setVector3Uniform("u_pointLight.specular", 1.0f * glm::vec3(1.0));
    backpackShader->setFloatUniform("u_pointLight.constant", 1.0f);
    backpackShader->setFloatUniform("u_pointLight.linear", 0.001f);
    backpackShader->setFloatUniform("u_pointLight.quadratic", 0.0002f);

    // spot light
    backpackShader->setFloatUniform("u_spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    backpackShader->setFloatUniform("u_spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
    backpackShader->setVector3Uniform("u_spotLight.ambient", 0.1f * glm::vec3(1.0));
    backpackShader->setVector3Uniform("u_spotLight.diffuse", 0.5f * glm::vec3(1.0));
    backpackShader->setVector3Uniform("u_spotLight.specular", 1.0f * glm::vec3(1.0));
    backpackShader->setFloatUniform("u_spotLight.constant", 1.0f);
    backpackShader->setFloatUniform("u_spotLight.linear", 0.07f);
    backpackShader->setFloatUniform("u_spotLight.quadratic", 0.0045f);
}