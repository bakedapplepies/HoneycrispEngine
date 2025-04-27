#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/managers/SceneManager.h"
#include "api/graphics/Camera.h"


HNCRSP_NAMESPACE_START

class GameLoop
{
private:
    Camera m_camera;

    bool m_continueProgram = true;
    float m_deltaTime = 0.0f;
    uint32_t m_lastFPS = 0;

public:
    GameLoop();
    ~GameLoop();
    void Start();

private:
    void _CalcFPS();
    void _ProcessInput();
};

HNCRSP_NAMESPACE_END