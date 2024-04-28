#pragma once

#include "src/pch/pch.h"
#include "src/managers/SceneManager.h"


HNCRSP_NAMESPACE_START

namespace RenderContext
{
    // ===== Data types =====
    
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
        int settingsWidth;
        float settingsWidthPercentage = 0.2f;

        glm::vec3 cameraDirection = glm::vec3(0, 0, -1.0f);  // initial camera direction or it would look at origin by default        
    };

    [[nodiscard]] CallbackData* StartUp_GetWindow();
}

HNCRSP_NAMESPACE_END