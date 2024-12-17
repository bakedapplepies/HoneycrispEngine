#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/graphics/Camera.h"


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

class RenderContext
{    
private:
    CallbackData m_callbackData;

public:
    HNCRSP_NODISCARD CallbackData* StartUp_GetWindow();
};

HNCRSP_NAMESPACE_END