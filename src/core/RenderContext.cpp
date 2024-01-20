#include "RenderContext.h"


HNCRSP_NAMESPACE_START

void RenderContext::StartUp()
{
    if (m_alreadyStarted)
        return;
        
    if(!glfwInit())
    {
        HNCRSP_TERMINATE("GLFW Initialization failed.");
    }
}

void RenderContext::ShutDown()
{
    glfwTerminate();
}

HNCRSP_NAMESPACE_END