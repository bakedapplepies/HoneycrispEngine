#pragma once

#include "GameInstance.h"


class GameEntry : public GameInstance
{
public:
    GameEntry() = default;

    void RegisterScenes() override final;
};