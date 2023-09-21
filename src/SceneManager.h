#pragma once

#include "pch/pch.h"
#include "Scene.h"


class SceneManager
{
private:
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;
    static SceneManager* sceneManager;

    static SceneManager& Get();
};

// namespace SceneManager
// {
//     std::vector<Scene> scenes;

// }