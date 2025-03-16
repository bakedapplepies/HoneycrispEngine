#include "api/game/GameEntry.h"
#include "Setup.h"
#include "scenes/DefaultSceneTwo.h"
#include "api/core/Timer.h"

void GameEntry::OnLoad()
{
    PreloadShaders();
}

void GameEntry::RegisterScenes()
{
    // Honeycrisp::Timer<std::chrono::milliseconds> timer("Scene Registration");
    uint32_t sponzaSceneID = AddScene<DefaultSceneTwo>("Sponza");
    Honeycrisp::g_SceneManager.SetActiveScene(sponzaSceneID);
}