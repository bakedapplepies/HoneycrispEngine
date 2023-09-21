#include "SceneManager.h"


SceneManager& SceneManager::Get()
{
    if (!sceneManager)
    {
        sceneManager = new SceneManager();
    }
    return *sceneManager;
}