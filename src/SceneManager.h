#pragma once

#include "pch/pch.h"
#include "Scene.h"


class SceneManager
{
private:
    std::vector<Scene> m_scenes;
    static SceneManager* m_sceneManager;
    
private:
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    static SceneManager& Get();

public:
    void Update() const
    {
        // for ()
    }
};

// namespace SceneManager
// {
//     std::vector<Scene> scenes;

// }