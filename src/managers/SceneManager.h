#pragma once

#include "src/managers/RenderContext.h"
#include "src/ecs/ECSManager.h"
#include "src/managers/ShaderManager.h"
#include "src/Scene.h"
#include "src/graphics/Camera.h"


HNCRSP_NAMESPACE_START

class SceneManager
{
private:
    uint32_t m_nextSceneIndex = 0;
    uint32_t m_activeSceneIndex = 0;
    std::unordered_map< uint32_t, std::unique_ptr<Scene> > m_scenesMap;
    CallbackData* m_callbackData;  // TODO: Temporary placement

public:
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;
    ~SceneManager() = default;

    void StartUp(CallbackData* callbackData);
    void ShutDown();

    void ClearAllScenes();
    void SetActiveScene(uint32_t index);
    uint32_t GetCurrentSceneIndex() const;
    const CallbackData* GetCallbackData() const;
    CallbackData* GetMutableCallbackData() const;

    void Update(const float& dt);
    void UpdateImGui();

    template <typename TScene>
    HNCRSP_NODISCARD uint32_t CreateScene()
    {
        static_assert(std::is_base_of<Scene, TScene>(), "SceneManager::CreateScene didn't receive object derived from Scene.");

        g_ECSManager.SceneCreated();  // This goes first so the function below can work
        g_ECSManager.SceneChanged(m_activeSceneIndex, m_nextSceneIndex);  // Replaces the entity list in the scene
        m_scenesMap[m_nextSceneIndex] = std::make_unique<TScene>();
        g_ECSManager.SceneChanged(m_nextSceneIndex, m_activeSceneIndex);

        return m_nextSceneIndex++;
    }
};

extern SceneManager g_SceneManager;

HNCRSP_NAMESPACE_END