#include "SceneManager.h"

SceneManager* SceneManager::m_sceneManager = nullptr;
SceneManager& SceneManager::Get()
{
    if (!m_sceneManager)
    {
        m_sceneManager = new SceneManager();
    }
    return *m_sceneManager;
}