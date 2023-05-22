#pragma once

#include "pch/pch.h"

#include "constants.h"
#include "Window.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    callbackData->windowWidth = width;
    callbackData->windowHeight = height;
}

void error_callback(int error, const char *msg)
{
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << '\n';
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
        if (front_back_mode[0] == GL_FILL && front_back_mode[1] == GL_FILL)
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
            // deltaX = callbackData->lastX;
            // deltaY = callbackData->lastY;
            callbackData->firstMouse = false;
            direction = glm::vec3(0.0f, 0.0f, 1.0f);
            return;
        }

        deltaX = xpos - callbackData->lastX;
        deltaY = ypos - callbackData->lastY;
        callbackData->lastX = xpos;
        callbackData->lastY = ypos;

        float sensitivity = 0.065f;
        deltaX *= sensitivity;
        deltaY *= sensitivity;

        yaw += deltaX;
        pitch += deltaY;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        direction.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        direction.y = sinf(glm::radians(-pitch));
        direction.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    }
}