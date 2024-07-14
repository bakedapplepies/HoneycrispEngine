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

    // glm::vec3 cameraDirection = glm::vec3(-1.0f, 0.0f, 0.0f);  // initial camera direction or it would look at origin by default        
    Camera camera = Camera(glm::vec3(-1.0f, 0.0f, 0.0f));
    glm::vec3 dirLightPos;
};

class RenderContext
{    
private:
    CallbackData m_callbackData;

public:
    [[nodiscard]] CallbackData* StartUp_GetWindow();
};

HNCRSP_NAMESPACE_END