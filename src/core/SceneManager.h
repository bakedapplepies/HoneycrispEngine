#pragma once

#include "src/pch/pch.h"
#include "src/Scene.h"


HNCRSP_NAMESPACE_START

class SceneManager
{
private:
    size_t _activeSceneIndex = 0;
    std::unordered_map< size_t, std::unique_ptr<Scene> > _scenesMap;
    static SceneManager* _instance;

private:
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

public:
    ~SceneManager() = default;
    void SetSceneBgColor(const glm::vec3& bgColor);
    glm::vec3 GetSceneBgColor();

public:
    template <typename T>
    void CreateScene(T&& t, size_t index)
    {
        static_assert(std::is_base_of<Scene, T>(), "Method SceneManager::CreateScene didn't receive a scene object.");
        if (_scenesMap[index])
        {
            HNCRSP_LOG_ERROR(fmt::format("Scene index[{}] already occupied.", index));
            return;
        }
        std::unique_ptr<T> temp_ptr = std::make_unique<T>(std::move(t));
        _scenesMap[index] = std::move(temp_ptr);
    }

    static SceneManager& Get();
    void Update();
    void ClearAllScenes();
    void SetActiveScene(size_t index);
};

HNCRSP_NAMESPACE_END