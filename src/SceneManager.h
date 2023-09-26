#pragma once

#include "pch/pch.h"
#include "Scene.h"


namespace SceneManager
{
    namespace
    {
        std::unordered_map< size_t, std::shared_ptr<Scene> > _scenesMap;
        size_t _sceneIndex;
    }
    extern std::vector< std::shared_ptr<Scene> > _scenes;

    template <typename T>
    extern void CreateScene(T&& t)
    {
        std::shared_ptr<T> temp_ptr = std::make_shared<T>(std::move(t));
        _scenes.push_back(temp_ptr);
    }
    extern void Update(Shader& shader);
    extern void ClearAllScenes();
}