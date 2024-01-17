#include "Application.h"



Application* Application::m_ContextWrap = nullptr;

Application::~Application()
{
    window.reset();
    glfwTerminate();
    HNCRSP_LOG_INFO("GLFW terminated.");
}

Application& Application::Get()
{
    if (!m_ContextWrap)
    {
        m_ContextWrap = new Application();
    }
    return *m_ContextWrap;
}

void Application::DeleteInstance()
{
    delete m_ContextWrap;
    m_ContextWrap = nullptr;
}

void Application::Run()
{
    if(!glfwInit())
    {
        HNCRSP_LOG_ERROR("GLFW Initialization failed.");
        return;
    }
    
    window = std::make_unique<Window>();
    window->Loop();
}