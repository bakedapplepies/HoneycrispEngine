#pragma once

#include "api/managers/SceneManager.h"


namespace Honeycrisp
{
    class Scene;
}

class GameInstance
{
protected:
    template <typename TScene, typename... Args>
    HNCRSP_NODISCARD uint32_t AddScene(const char* scene_name, Args&&... args)
    {
        return Honeycrisp::g_SceneManager.CreateScene<TScene>(scene_name, std::forward<Args>(args)...);
    }

public:
    GameInstance() = default;
    GameInstance(const GameInstance&) = delete;
    GameInstance& operator=(const GameInstance&) = delete;
    GameInstance(GameInstance&&) = delete;
    GameInstance& operator=(GameInstance&&) = delete;
    ~GameInstance() = default;

    virtual void RegisterScenes() = 0;
};