#pragma once

#include "api/ecs/ECSManager.h"
#include "api/scene/Scene.h"

HNCRSP_NAMESPACE_START

class GameLoop;
class ImGuiManager;
class SceneManager
{
private:
    uint32_t m_nextSceneIndex;
    uint32_t m_activeSceneIndex;
    std::unordered_map< uint32_t, std::unique_ptr<Scene> > m_scenesMap;

public:
    SceneManager();
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;
    ~SceneManager() = default;

    HNCRSP_NODISCARD uint64_t StartUp();
    void ShutDown(uint64_t key);

    // Scene functions
    void SetActiveScene(uint32_t index);
    uint32_t GetCurrentSceneIndex() const;

    // Utilities
    const DirectionalLight* GetCurrentDirectionalLight();

    template <typename TScene, typename... Args>
    HNCRSP_NODISCARD uint32_t CreateScene(const char* scene_name, Args&&... args)
    {
        g_ECSManager._SceneCreated();  // This goes first so the function below can work
        g_ECSManager._SceneChanged(m_activeSceneIndex, m_nextSceneIndex);  // Replaces the entity list in the scene
        m_scenesMap[m_nextSceneIndex] = std::make_unique<TScene>(scene_name, std::forward<Args>(args)...);
        g_ECSManager._SceneChanged(m_nextSceneIndex, m_activeSceneIndex);

        return m_nextSceneIndex++;
    }

private:
    friend GameLoop;
    friend ImGuiManager;
    // Update Scenes and their relevant ImGui
    void _Update(const float& dt);
    void _UpdateImGui();
};

extern SceneManager g_SceneManager;

HNCRSP_NAMESPACE_END