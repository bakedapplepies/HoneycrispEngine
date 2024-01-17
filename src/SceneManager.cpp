#include "SceneManager.h"



SceneManager* SceneManager::_instance = nullptr;

SceneManager& SceneManager::Get()
{
    if (!_instance)
    {
        _instance = new SceneManager();
    }
    return *_instance;
}

void SceneManager::Update()
{
    // maybe add if-statements to check availability
    _scenesMap[_activeSceneIndex]->onUpdate();
}

void SceneManager::ClearAllScenes()
{
    _scenesMap.clear();
}

void SceneManager::SetActiveScene(size_t index)
{
    if (!_scenesMap[index])
    {
        HNCRSP_LOG_ERROR(fmt::format("Scene index [{}] not found.", index));
        return;
    }
    _activeSceneIndex = index;
}

void SceneManager::SetSceneBgColor(const glm::vec3& bgColor)
{
    _scenesMap[_activeSceneIndex]->bgColor = bgColor;
}

glm::vec3 SceneManager::GetSceneBgColor()
{
    return _scenesMap[_activeSceneIndex]->bgColor;
}