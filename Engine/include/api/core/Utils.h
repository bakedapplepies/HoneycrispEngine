#pragma once

#include "api/pch/hncrsp_pch.h"

HNCRSP_NAMESPACE_START

struct CallbackData
{
    bool capFPS = true;

    bool showMouse = false;
    bool firstMouse = true;
    float lastX;
    float lastY;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;

    int windowWidth;
    int windowHeight;
};

HNCRSP_NODISCARD extern CallbackData* GetCallbackData();

HNCRSP_NAMESPACE_END