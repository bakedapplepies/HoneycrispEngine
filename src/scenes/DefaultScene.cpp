#include "DefaultScene.h"

#include "../utils/utils.h"
#include "src/core/Texture2DManager.h"


#define RAND rand()/double(RAND_MAX)

using namespace Honeycrisp;

DefaultScene::DefaultScene()
{
    // Texture2D& texture2d = Texture2DManager::getTexture2D(FileSystem::Path("resources/textures/grass_textures.png"));
    shader = std::make_shared<Shader>(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/PhongShadingFragment.glsl")
    );

    cube = CreateObject<Cube, EObjectMovement::NONE>();
    cube->setShader(shader);
    cube->addTransform(Transform(glm::vec3(-1.0f, -3.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    cube->addTransform(Transform(glm::vec3(2.0f, 0.0f, 2.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    cube->addTransform(Transform(glm::vec3(-3.0f, 3.0f, 4.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

    customMesh = CreateObject<Mesh, EObjectMovement::NONE>();

    customMesh->vertices = std::vector<glm::vec3> {
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

    customMesh->colors = std::vector<glm::vec3> {
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

    customMesh->normals = std::vector<glm::vec3> {
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

    customMesh->indices = std::vector<GLuint> {
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

    customMesh->ConstructMesh();
    customMesh->addTransform(Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

    SetInitialUniforms();
}

DefaultScene::~DefaultScene()
{
}

void DefaultScene::onUpdate()
{
    Draw();
}

void DefaultScene::SetInitialUniforms(void)
{
    // lighting
    shader->setIntUniform("u_material.albedo", Texture2DManager::mainTextureMap->getTextureUnit());
    shader->setIntUniform("u_material.specular", Texture2DManager::mainTextureSpecularMap->getTextureUnit());
    shader->setFloatUniform("u_material.shininess", 32.0f);

    // dir light
    shader->setVector3Uniform("u_dirLight.direction", glm::normalize(glm::vec3(0, -1, 0)));
    shader->setVector3Uniform("u_dirLight.ambient", glm::vec3(1, 1, 1) * 0.1f);
    shader->setVector3Uniform("u_dirLight.diffuse", glm::vec3(1, 1, 1) * 0.7f);
    shader->setVector3Uniform("u_dirLight.specular", glm::vec3(1, 1, 1));

    // point light
    shader->setVector3Uniform("u_pointLight.position", glm::vec3(10, 2, 10));
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
}