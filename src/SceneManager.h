#pragma once

#include "pch/pch.h"


class SceneManager
{
private:
    SceneManager();
    SceneManager(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;
    SceneManager* sceneManager;

    static SceneManager& Get()
    {
        return *instance;
    }
};