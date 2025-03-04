#include "Callbacks.h"
#include "src/RenderContext.h"
#include "src/WindowHandler.h"
#include "api/managers/SceneManager.h"
#include "api/systems/Renderer.h"


HNCRSP_NAMESPACE_START

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // static CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    
    // callbackData->windowWidth = width;
    // callbackData->windowHeight = height;  // to reconstruct perspective matrix
    
    // glViewport(
    //     0,
    //     0,
    //     callbackData->windowWidth * (1.0f - callbackData->settingsWidthPercentage),
    //     callbackData->windowHeight
    // );
}

void error_callback(int error, const char* msg)
{
    if (error == GLFW_NOT_INITIALIZED) return;  // for checking if opengl is there to properly delete resources

    HNCRSP_ERROR("({}): {}", error, msg);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            // Exit windows
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
                break;

            // Toggle wireframes
            case GLFW_KEY_M:
                static bool wireframe = false;
                wireframe = !wireframe;
                glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
                break;

            // Toggle FPS-cap
            case GLFW_KEY_RIGHT_ALT:
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
                HNCRSP_INFO("Toggle frame swap interval: {}", callbackData->capFPS);
                break;

            // Toggle cursor
            case GLFW_KEY_LEFT_ALT:
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
                break;

            // Switch scene #1
            case GLFW_KEY_1:
                g_SceneManager.SetActiveScene(0);
                break;

            // Switch scene #2
            case GLFW_KEY_2:
                g_SceneManager.SetActiveScene(1);
                break;

            // Switch scene #2
            case GLFW_KEY_3:
                g_SceneManager.SetActiveScene(2);
                break;

            default:
                break;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos_double, double ypos_double)
{
    static CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    static Camera* camera = g_ECSManager.GetSystem<Renderer>()->GetCameraMutable();
    
    if (!callbackData->showMouse)
    {
        float xpos = static_cast<float>(xpos_double);
        float ypos = static_cast<float>(ypos_double);
        float deltaX;
        float deltaY;

        float& yaw = callbackData->yaw;
        float& pitch = callbackData->pitch;
        glm::vec3& direction = camera->direction;

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

HNCRSP_NAMESPACE_END