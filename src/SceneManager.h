#pragma once

#include "pch/pch.h"
#include "Scene.h"


class SceneManager
{
private:
    size_t _activeSceneIndex;
    std::unordered_map< size_t, std::shared_ptr<Scene> > _scenesMap;
    static SceneManager* _instance;

private:
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

public:
    ~SceneManager() = default;

public:
    template <typename T>
    void CreateScene(T&& t, size_t index)
    {
        std::shared_ptr<T> temp_ptr = std::make_shared<T>(std::move(t));
        _scenesMap[index] = temp_ptr;
    }

    static SceneManager& Get();
    void Update(Shader& shader);
    void ClearAllScenes();
    void SetActiveScene(size_t index);
};