#pragma once

#include "src/managers/SceneManager.h"


HNCRSP_NAMESPACE_START

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void error_callback(int error, const char* msg);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos_double, double ypos_double);

HNCRSP_NAMESPACE_END