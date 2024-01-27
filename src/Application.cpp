#include "Application.h"
#include "src/core/GLFWContext.h"
#include "src/core/RenderContext.h"
#include "src/core/ImGuiManager.h"
#include "src/core/Texture2DManager.h"
#include "src/core/SceneManager.h"


HNCRSP_NAMESPACE_START

// dependencies can be distributed via depedency injection
void Application::Run()
{    
    GLFWContext::StartUp();
    GLFWwindow* glfwWindow = RenderContext::StartUp_GetWindow();
    Window window;
    window.StartUp(glfwWindow);
    ImGuiManager::StartUp(glfwWindow);
    Texture2DManager::StartUp();

    window.Loop();

    Texture2DManager::ShutDown();
    ImGuiManager::ShutDown();
    SceneManager::Get().ClearAllScenes();
    GLFWContext::ShutDown();
}

HNCRSP_NAMESPACE_END