#include "Application.h"
#include "Debug.h"


Application* Application::ContextWrap = nullptr;

Application::~Application()
{
    window.reset();
    glfwTerminate();
    Debug::Log("GLFW terminated safely.");
}

Application& Application::Get()
{
    if (!ContextWrap)
    {
        ContextWrap = new Application();
    }
    return *ContextWrap;
}

void Application::Run()
{
    if(!glfwInit())
    {
        Debug::Error("GLFW Initialization failed.");
        return;
    }
    
    window = std::make_unique<Window>();
    window->Loop();
}