#include "DefaultScene.h"
#include "../Debug.h"


DefaultScene::DefaultScene()
{
    cube = CreateObject(Cube(), EObjectRenderType::STATIC);
    cube->AddPosition(glm::vec3(-1.0f, -3.0f, -1.0f));
    cube->AddPosition(glm::vec3(2.0f, 0.0f, 2.0f));
    cube->AddPosition(glm::vec3(-3.0f, 3.0f, 4.0f));
}

void DefaultScene::OnUpdate(Shader& shader)
{
    cube->Draw(shader);
}