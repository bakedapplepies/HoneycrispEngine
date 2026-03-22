#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/Defines.h"
#include "Camera.h"

HNCRSP_NAMESPACE_START

struct GLFWUserData
{
    Camera* cameraInControl;
};

extern void FramebufferSizeCallback(GLFWwindow* window,
                                    int width,
                                    int height);
extern void ErrorCallback(int error_code, const char* description);
extern void MouseCallback(GLFWwindow* window, double xpos_double, double ypos_double);

HNCRSP_NAMESPACE_END