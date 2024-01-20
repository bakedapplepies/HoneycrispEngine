#include "Application.h"
#include "src/core/RenderContext.h"
#include "src/core/Texture2DManager.h"
#include "src/core/SceneManager.h"


HNCRSP_NAMESPACE_START

// dependencies can be distributed via depedency injection
void Application::Run()
{    
    RenderContext::StartUp();

    std::unique_ptr<Window> window = std::make_unique<Window>();
    Texture2DManager::StartUp();
    window->Loop();
    Texture2DManager::ShutDown();
    window.reset();

    SceneManager::Get().ClearAllScenes();

    RenderContext::ShutDown();
}

HNCRSP_NAMESPACE_END