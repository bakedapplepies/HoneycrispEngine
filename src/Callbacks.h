#pragma once

#include "pch/pch.h"

#include "Window.h"
#include "Debug.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    callbackData->windowWidth = width;
    callbackData->windowHeight = height;  // to reconstruct perspective matrix
}

void error_callback(int error, const char* msg)
{
    std::string errorMsg = fmt::format("({}): {}", error, msg);
    Debug::Error(errorMsg);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));

    // Exit window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Toggle wireframes
    else if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        GLint front_back_mode[2];
        glGetIntegerv(GL_POLYGON_MODE, front_back_mode);
        if (front_back_mode[0] == GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    else if (key == GLFW_KEY_RIGHT_ALT && action == GLFW_PRESS)
    {
        if (!callbackData->capFPS)
        {
            glfwSwapInterval(1);
            callbackData->capFPS = true;
        }
        else
        {
            glfwSwapInterval(0);
            callbackData->capFPS = false;
        }
    }

    else if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
    {   
        if (!callbackData->showMouse)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            callbackData->showMouse = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            callbackData->showMouse = false;
            callbackData->firstMouse = true;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos_double, double ypos_double)
{
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    static float offsetX, offsetY;

    if (!callbackData->showMouse)
    {
        float xpos = static_cast<float>(xpos_double);
        float ypos = static_cast<float>(ypos_double);
        float deltaX;
        float deltaY;

        float& yaw = callbackData->yaw;
        float& pitch = callbackData->pitch;
        glm::vec3& direction = callbackData->cameraDirection;

        if (callbackData->firstMouse)
        {
            callbackData->lastX = xpos;
            callbackData->lastY = ypos;
            callbackData->firstMouse = false;
            return;
        }

        deltaX = xpos - callbackData->lastX;
        deltaY = ypos - callbackData->lastY;
        callbackData->lastX = xpos;
        callbackData->lastY = ypos;

        float sensitivity = 0.065f;  // adjust in settings later -> put in CallbackData
        deltaX *= sensitivity;
        deltaY *= sensitivity;

        yaw += deltaX;
        pitch += deltaY;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        direction.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        direction.y = sinf(glm::radians(-pitch));  // since going up means Y is going down
        direction.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    }
}