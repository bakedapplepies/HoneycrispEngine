#pragma once

#include "GameInstance.h"

class GameEntry : public GameInstance
{
public:
    GameEntry() = default;

    void OnLoad() override final;
    void RegisterScenes() override final;
};