#include "OGLWrapper.h"
#include "Debug.h"


OGLWrapper* OGLWrapper::ContextWrap = nullptr;

OGLWrapper::~OGLWrapper()
{
    delete window;
    glfwTerminate();
    Debug::Log("GLFW terminated safely.");
}

OGLWrapper* OGLWrapper::Get()
{
    if (!ContextWrap)
    {
        ContextWrap = new OGLWrapper();
    }
    return ContextWrap;
}

int OGLWrapper::Run()
{
    if(!glfwInit())
    {
        Debug::Error("GLFW Initialization failed.");
        return -1;
    }
    window = new Window();
    window->Loop();
    return 0;
}