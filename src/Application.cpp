#include "Application.h"
#include "src/core/GLFWContext.h"
#include "src/core/RenderContext.h"
#include "src/core/ImGuiManager.h"
#include "src/core/Texture2DManager.h"
#include "src/core/SceneManager.h"
#include "src/Window.h"


HNCRSP_NAMESPACE_START

void Application::Run()
{
    // get dependencies via depedency injection
    // SetProcessDPIAware();

    // uint32_t dpi_x, dpi_y;
    // PFN_GetDpiForMonitor();
    // SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    GLFWContext::StartUp();
    auto [glfwWindow, callbackData] = RenderContext::WindowPtr_and_CallbackDataPtr {
        RenderContext::StartUp_GetWindow()
    };
    Window window;
    window.StartUp(glfwWindow, callbackData);
    ImGuiManager::StartUp(glfwWindow);
    Texture2DManager::StartUp();

    window.Loop();

    Texture2DManager::ShutDown();
    ImGuiManager::ShutDown();
    SceneManager::Get().ClearAllScenes();
    GLFWContext::ShutDown();

    // Window object can be deleted of the stack easily
}

HNCRSP_NAMESPACE_END