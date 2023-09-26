#include "SceneManager.h"
#include "Debug.h"


std::vector< std::shared_ptr<Scene> > SceneManager::_scenes;

void SceneManager::Update(Shader& shader)
{
    for (std::shared_ptr<Scene> scene : _scenes)
    {
        scene->OnUpdate(shader);
    }
}

void SceneManager::ClearAllScenes()
{
    _scenes.clear();
}