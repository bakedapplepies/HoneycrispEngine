#include "DefaultScene.h"
#include "../Debug.h"


DefaultScene::DefaultScene()
{
    shader = std::make_shared<Shader>(
        "../resources/shaders/vertex.glsl",
        "../resources/shaders/fragment.glsl"
    );

    cube = CreateObject(Cube(), EObjectRenderType::STATIC, shader);
    cube->AddPosition(glm::vec3(-1.0f, -3.0f, -1.0f));
    cube->AddPosition(glm::vec3(2.0f, 0.0f, 2.0f));
    cube->AddPosition(glm::vec3(-3.0f, 3.0f, 4.0f));

    SetInitialUniforms();
}

void DefaultScene::OnUpdate(Shader& shader)
{
    Draw();
}

void DefaultScene::SetInitialUniforms(void)
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