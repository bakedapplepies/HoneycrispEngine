#include "SceneManager.h"
#include "Debug.h"


SceneManager* SceneManager::_instance = nullptr;

SceneManager& SceneManager::Get()
{
    if (!_instance)
    {
        _instance = new SceneManager();
    }
    return *_instance;
}

void SceneManager::Update(Shader& shader)
{
    _scenesMap[_activeSceneIndex]->OnUpdate(shader);
}

void SceneManager::ClearAllScenes()
{
    _scenesMap.clear();
}

void SceneManager::SetActiveScene(size_t index)
{
    if (!_scenesMap[index])
    {
        Debug::Error(fmt::format("Scene index [{}] not found.", index));
        return;
    }
    _activeSceneIndex = index;
}