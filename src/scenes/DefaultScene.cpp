#include "DefaultScene.h"
#include "../Debug.h"


DefaultScene::DefaultScene()
{
    cube = CreateObject(Cube(), EObjectRenderType::STATIC);
    cube->AddPosition(glm::vec3(-1.0f, -3.0f, -1.0f));
}

void DefaultScene::OnUpdate(Shader& shader)
{
    cube->Draw(shader);
}