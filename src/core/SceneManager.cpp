#include "SceneManager.h"


HNCRSP_NAMESPACE_START

SceneManager* SceneManager::m_instance = nullptr;

SceneManager& SceneManager::Get()
{
    if (!m_instance)
    {
        m_instance = new SceneManager();
    }
    return *m_instance;
}

void SceneManager::Update()
{
    // maybe add if-statements to check availability
    m_scenesMap[m_activeSceneIndex]->onUpdate();
}

void SceneManager::ClearAllScenes()
{
    m_scenesMap.clear();
}

void SceneManager::SetActiveScene(size_t index)
{
    if (!m_scenesMap[index])
    {
        HNCRSP_LOG_ERROR(fmt::format("Scene index [{}] not found.", index));
        return;
    }
    m_activeSceneIndex = index;
}

void SceneManager::SetSceneBgColor(const glm::vec3& bgColor)
{
    m_scenesMap[m_activeSceneIndex]->bgColor = bgColor;
}

glm::vec3 SceneManager::GetSceneBgColor()
{
    return m_scenesMap[m_activeSceneIndex]->bgColor;
}

HNCRSP_NAMESPACE_END