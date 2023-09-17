#include "Application.h"
#include "Debug.h"


Application* Application::ContextWrap = nullptr;

Application::~Application()
{
    delete window;
    glfwTerminate();
    Debug::Log("GLFW terminated safely.");
}

Application* Application::Get()
{
    if (!ContextWrap)
    {
        ContextWrap = new Application();
    }
    return ContextWrap;
}

int Application::Run()
{
    if(!glfwInit())
    {
        Debug::Error("GLFW Initialization failed.");
        return -1;
    }
    
    window = new Window();
    window->Loop(ContextWrap);
    return 0;
}

void Application::OnUpdate()
{

}

void Application::OnImGUI()
{

}