#include "GLFWContext.h"


HNCRSP_NAMESPACE_START

void GLFWContext::StartUp()
{
    if (glfwGetCurrentContext())
    {
        HNCRSP_LOG_ERROR("GLFW already initialized.");
        return;
    }
        
    if (!glfwInit())
    {
        HNCRSP_TERMINATE("GLFW Initialization failed.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
}

void GLFWContext::ShutDown()
{
    glfwTerminate();
}

HNCRSP_NAMESPACE_END