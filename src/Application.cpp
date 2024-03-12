#include "Application.h"
#include "src/core/GLFWContext.h"
#include "src/core/RenderContext.h"
#include "src/core/ImGuiManager.h"
#include "src/core/Texture2DManager.h"
#include "src/core/ShaderManager.h"
#include "src/core/SceneManager.h"
#include "src/Callbacks.h"
#include "src/Window.h"
#include "src/ecs/ECSManager.h"

#include "src/components/Transform.h"
#include "src/components/MeshData.h"

#include "src/systems/Renderer.h"


HNCRSP_NAMESPACE_START

void Application::Run()
{
    GLFWContext::StartUp();
    RenderContext::CallbackData* callbackData = RenderContext::StartUp_GetWindow();
    g_ImGuiManager.StartUp();
    g_ShaderManager.StartUp();
    g_Texture2DManager.StartUp();
    g_SceneManager.StartUp(Application_RegisterSystems, Application_RegisterComponents);


    // resources in Window class are mostly managed by other managers
    // regular members don't have to be processed
    Window window;
    window.StartUp(callbackData);

    window.Loop();


    g_SceneManager.ShutDown();
    g_Texture2DManager.ShutDown();
    g_ShaderManager.ShutDown();
    g_ImGuiManager.ShutDown();
    g_SceneManager.ClearAllScenes();
    GLFWContext::ShutDown();
}

void Application_RegisterComponents()
{
    g_ECSManager->RegisterComponent<Transform>();
    g_ECSManager->RegisterComponent<MeshData>();
}

void Application_RegisterSystems()
{
    ComponentBitset renderer_component_bitset;
    renderer_component_bitset.set(GetBitIndex<MeshData>());
    renderer_component_bitset.set(GetBitIndex<Transform>());
    g_ECSManager->RegisterSystem<Renderer>(renderer_component_bitset);
}

HNCRSP_NAMESPACE_END