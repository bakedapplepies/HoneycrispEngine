#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

struct GLFWUserData
{
    Camera* cameraInControl;
};

extern void FramebufferSizeCallback(GLFWwindow* window,
                                    int width,
                                    int height);
extern void ErrorCallback(int error_code, const char* description);
extern void MouseCallback(GLFWwindow* window, double xpos_double, double ypos_double);