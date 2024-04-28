#include "SceneManager.h"


HNCRSP_NAMESPACE_START

SceneManager g_SceneManager;

void SceneManager::StartUp(
    const std::function<void()>& application_ECS_register_systems,
    const std::function<void()>& application_ECS_register_components
) {
    m_application_ECS_register_systems = application_ECS_register_systems;
    m_application_ECS_register_components = application_ECS_register_components;
}

void SceneManager::ShutDown()
{
    m_scenesMap.clear();
    
    for (auto& i : m_ECSManagers)
    {
        ECSManager& ecsManager = i.second;
        ecsManager.ShutDown();
    }
}

void SceneManager::Update(const float& dt)
{
    m_scenesMap[m_activeSceneIndex]->OnUpdate(dt);
}

void SceneManager::UpdateImGui()
{
    // same comment as this->Update
    m_scenesMap[m_activeSceneIndex]->OnImGui();
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
    g_ECSManager = &m_ECSManagers[m_activeSceneIndex];
}

size_t SceneManager::GetCurrentSceneIndex() const
{
    return m_activeSceneIndex;
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