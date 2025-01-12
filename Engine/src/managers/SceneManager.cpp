#include "api/managers/SceneManager.h"
#include "api/systems/Renderer.h"
#include "api/core/Random.h"

HNCRSP_NAMESPACE_START

SceneManager g_SceneManager;

static uint64_t s_key;
uint64_t SceneManager::StartUp()
{
    static bool startedUp = false;
    if (startedUp)
    {
        HNCRSP_ERROR("SceneManager already initialized!");
        return UINT64_MAX;
    }
    startedUp = true;
    s_key = Random<uint64_t>::dist(0ULL, UINT64_MAX - 1);
    return s_key;  // key hidden by implementation
}

void SceneManager::ShutDown(uint64_t key)
{
    if (key != s_key)
    {
        HNCRSP_ERROR("SceneManager shutdown key invalid!");
        return;
    }
    m_scenesMap.clear();
}

SceneManager::SceneManager()
    : m_activeSceneIndex(0), m_nextSceneIndex(0)
{}

void SceneManager::_Update(const float& dt)
{
    m_scenesMap[m_activeSceneIndex]->OnUpdate(dt);
}

void SceneManager::_UpdateImGui()
{
    // same comment as this->Update
    m_scenesMap[m_activeSceneIndex]->OnImGui();
}

void SceneManager::SetActiveScene(uint32_t index)
{
    if (!m_scenesMap[index])
    {
        HNCRSP_ERROR("Scene index [{}] not found.", index);
        return;
    }

    g_ECSManager._SceneChanged(m_activeSceneIndex, index);
    m_activeSceneIndex = index;
    m_scenesMap[m_activeSceneIndex]->_ReconfigureAllShaders();

    Renderer* renderer = g_ECSManager.GetSystem<Renderer>();
    renderer->SetCubemap(m_scenesMap[m_activeSceneIndex]->m_cubemap.get());
}

uint32_t SceneManager::GetCurrentSceneIndex() const
{
    return m_activeSceneIndex;
}

const DirectionalLight* SceneManager::GetCurrentDirectionalLight()
{
    return m_scenesMap[m_activeSceneIndex]->m_lightContainer.directionalLights[0].get();
}

HNCRSP_NAMESPACE_END