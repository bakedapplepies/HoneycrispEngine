#pragma once

#include "src/pch/pch.h"
#include "src/Callbacks.h"


HNCRSP_NAMESPACE_START

namespace RenderContext
{
    // ===== Data types =====
    
    struct CallbackData
    {
        bool capFPS = true;
        bool showMouse = false;
        bool firstMouse = true;
        int windowHeight;
        int windowWidth;
        float lastX;
        float lastY;
        float yaw = -90.0f;
        float pitch = 0.0f;
        float fov = 45.0f;
        glm::vec3 cameraDirection = glm::vec3(0, 0, -1.0f);  // initial camera direction or it would look at origin by default
    };

    struct WindowPtr_and_CallbackDataPtr
    {
        GLFWwindow* glfwWindow;
        CallbackData* callbackData;
    };

    // ===== Namespace stuff =====

    namespace
    {
        CallbackData callbackData;
    }

    [[nodiscard]] WindowPtr_and_CallbackDataPtr StartUp_GetWindow();
}

HNCRSP_NAMESPACE_END