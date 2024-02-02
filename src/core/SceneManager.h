#pragma once

#include "src/types/Singleton.h"
#include "src/Scene.h"


HNCRSP_NAMESPACE_START

class SceneManager : public Singleton
{
private:
    size_t m_activeSceneIndex = 0;
    std::unordered_map< size_t, std::unique_ptr<Scene> > m_scenesMap;
    static SceneManager* m_instance;

private:
    SceneManager() = default;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

public:
    ~SceneManager() = default;
    void SetSceneBgColor(const glm::vec3& bgColor);
    glm::vec3 GetSceneBgColor();

    template <typename T>
    void CreateScene(T&& t, size_t index)
    {
        static_assert(std::is_base_of<Scene, T>(), "Method SceneManager::CreateScene didn't receive a scene object.");
        if (m_scenesMap[index])
        {
            HNCRSP_LOG_ERROR(fmt::format("Scene index[{}] already occupied.", index));
            return;
        }
        std::unique_ptr<T> temp_ptr = std::make_unique<T>(std::move(t));
        m_scenesMap[index] = std::move(temp_ptr);
    }

    static SceneManager& Get();
    void Update();
    void UpdateImGui();
    void ClearAllScenes();
    void SetActiveScene(size_t index);
};

HNCRSP_NAMESPACE_END