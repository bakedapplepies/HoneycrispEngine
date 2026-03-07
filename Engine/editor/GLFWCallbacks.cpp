#include "GLFWCallbacks.h"

#include <fmt/format.h>
#include <glm/glm.hpp>

void FramebufferSizeCallback(GLFWwindow* window,
                             int width,
                             int height)
{
    glViewport(0, 0, width, height);
}

void ErrorCallback(int error_code, const char* description)
{
    fmt::println("[{}]: {}", error_code, description);
}

void MouseCallback(GLFWwindow *window, double xpos_double, double ypos_double)
{
    static GLFWUserData* userData = static_cast<GLFWUserData*>(glfwGetWindowUserPointer(window));
    static Camera* camera = userData->cameraInControl;

    static float lastX = 0.0f;
    static float lastY = 0.0f;
    static float yaw = -90.0f;
    static float pitch = 0.0f;
    
    if (true)  // not showing mouse logic
    {
        float xpos = static_cast<float>(xpos_double);
        float ypos = static_cast<float>(ypos_double);
        float deltaX;
        float deltaY;

        glm::vec3& direction = camera->viewDir;

        // if (callbackData->firstMouse)
        // {
        //     callbackData->lastX = xpos;
        //     callbackData->lastY = ypos;
        //     callbackData->firstMouse = false;
        //     return;
        // }

        deltaX = xpos - lastX;
        deltaY = ypos - lastY;
        lastX = xpos;
        lastY = ypos;

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
        direction.y = sinf(glm::radians(-pitch));  // since going up means Y is going down
        direction.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    }
}