#include "Application.h"
#include "Debug.h"


std::shared_ptr<Application> Application::ContextWrap = nullptr;

Application::~Application()
{
    delete window;
    glfwTerminate();
    Debug::Log("GLFW terminated safely.");
}

std::shared_ptr<Application> Application::Get()
{
    if (!ContextWrap)
    {
        ContextWrap = std::make_shared<Application>();
    }
    return ContextWrap;
}

void Application::Run()
{
    if(!glfwInit())
    {
        Debug::Error("GLFW Initialization failed.");
        return;
    }
    
    window = new Window();
    window->Loop(ContextWrap);
}

void Application::OnUpdate()
{

}

void Application::OnImGUI()
{

}