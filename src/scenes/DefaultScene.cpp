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
}

void DefaultScene::OnUpdate(Shader& shader)
{
    Draw();
}