#pragma once

#include "src/ecs/ECSManager.h"
#include "src/managers/ShaderManager.h"
#include "src/Scene.h"


HNCRSP_NAMESPACE_START

class SceneManager
{
private:
    size_t m_nextSceneIndex = 0;
    size_t m_activeSceneIndex = 0;
    std::unordered_map< size_t, std::unique_ptr<Scene> > m_scenesMap;
    std::unordered_map< size_t, ECSManager > m_ECSManagers;

    // application is high-level, this is just some kind of forward declaration stuff
    // so the compiler can figure out the function ptrs out later
    std::function<void()> m_application_ECS_register_systems;
    std::function<void()> m_application_ECS_register_components;

public:
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;
    ~SceneManager() = default;

    void StartUp(
        const std::function<void()>& application_ECS_register_systems,
        const std::function<void()>& application_ECS_register_components
    );
    void ShutDown();

    void SetSceneBgColor(const glm::vec3& bgColor);
    glm::vec3 GetSceneBgColor();
    void ClearAllScenes();
    void SetActiveScene(size_t index);
    size_t GetCurrentSceneIndex() const;

    void Update(const float& dt);
    void UpdateImGui();

    template <typename TScene>
    [[nodiscard]] size_t CreateScene()
    {
        static_assert(std::is_base_of<Scene, TScene>(), "SceneManager::CreateScene didn't receive a scene object.");

        m_ECSManagers[m_nextSceneIndex] = ECSManager();
        g_ECSManager = &m_ECSManagers[m_nextSceneIndex];
        g_ECSManager->StartUp();
        m_application_ECS_register_components();
        m_application_ECS_register_systems();

        size_t tempSceneIndex = m_activeSceneIndex;  // just for easier debugging between scenes
        m_activeSceneIndex = m_nextSceneIndex;

        m_scenesMap[m_nextSceneIndex] = std::make_unique<TScene>();
        m_activeSceneIndex = tempSceneIndex;
        g_ECSManager = &m_ECSManagers[m_activeSceneIndex];
        return m_nextSceneIndex++;
    }
};

extern SceneManager g_SceneManager;

HNCRSP_NAMESPACE_END