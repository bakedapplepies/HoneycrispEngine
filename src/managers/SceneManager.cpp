#include "SceneManager.h"


HNCRSP_NAMESPACE_START

SceneManager g_SceneManager;

void SceneManager::StartUp(
    CallbackData* callbackData
) {
    m_callbackData = callbackData;
}

void SceneManager::ShutDown()
{
    m_scenesMap.clear();
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

void SceneManager::SetActiveScene(uint32_t index)
{
    if (!m_scenesMap[index])
    {
        HNCRSP_LOG_ERROR(fmt::format("Scene index [{}] not found.", index));
        return;
    }

    g_ECSManager.SceneChanged(m_activeSceneIndex, index);
    m_activeSceneIndex = index;
    m_scenesMap[m_activeSceneIndex]->_ReconfigureAllShaders();

    Renderer* renderer = g_ECSManager.GetSystem<Renderer>();
    renderer->SetCubemap(m_scenesMap[m_activeSceneIndex]->cubemap.get());
}

uint32_t SceneManager::GetCurrentSceneIndex() const
{
    return m_activeSceneIndex;
}

const CallbackData* SceneManager::GetCallbackData() const
{
    return m_callbackData;
}

CallbackData* SceneManager::GetMutableCallbackData() const
{
    return m_callbackData;
}

HNCRSP_NAMESPACE_END