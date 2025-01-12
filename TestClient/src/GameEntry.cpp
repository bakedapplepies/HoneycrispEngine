#include "api/game/GameEntry.h"
#include "scenes/DefaultSceneTwo.h"


void GameEntry::RegisterScenes()
{
    uint32_t sponzaSceneID = AddScene<DefaultSceneTwo>("Sponza");
    Honeycrisp::g_SceneManager.SetActiveScene(sponzaSceneID);
}