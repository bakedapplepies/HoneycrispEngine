#include "Application.h"

// Managers
#include "src/managers/GLFWContext.h"
#include "src/managers/RenderContext.h"
#include "src/managers/ImGuiManager.h"
#include "src/managers/Texture2DManager.h"
#include "src/managers/ShaderManager.h"
#include "src/managers/SceneManager.h"
#include "src/Callbacks.h"
#include "src/Window.h"
#include "src/ecs/ECSManager.h"

// Components
#include "src/components/Transform.h"
#include "src/components/DrawData.h"

// ECS Systems
#include "src/systems/Renderer.h"


HNCRSP_NAMESPACE_START

static void GetExtensions();
void Application::Run()  // this is where the main control flow happens
{
    // std::filesystem::create_directory("error");
    // std::ofstream stderr_redirect("error/error.txt", std::ios::out | std::ios::trunc);
    // std::streambuf* cerrBuf = std::cerr.rdbuf();  // save cerr buffer
    // std::cerr.rdbuf(stderr_redirect.rdbuf());  // redirect to file buffer

    GLFWContext glfwContext;
    glfwContext.StartUp();

    RenderContext renderContext;
    CallbackData* callbackData = renderContext.StartUp_GetWindow();  // TODO: This is pretty akward design

    GetExtensions();

    g_ImGuiManager.StartUp();
    g_ShaderManager.StartUp();
    g_Texture2DManager.StartUp();
    g_SceneManager.StartUp(callbackData);
    Application_RegisterComponents();
    Application_RegisterSystems();

    // resources in Window class are mostly managed by other managers
    // post-deletion of resources is mostly trivial
    Window window(callbackData);
    window.Loop();

    g_SceneManager.ClearAllScenes();
    g_SceneManager.ShutDown();
    g_Texture2DManager.ShutDown();
    g_ShaderManager.ShutDown();
    g_ImGuiManager.ShutDown();
    glfwContext.ShutDown();

    // std::cerr.rdbuf(cerrBuf);  // reset cerr
    // stderr_redirect.close();
}

void Application_RegisterComponents()
{
    g_ECSManager.RegisterComponent<Transform>();
    g_ECSManager.RegisterComponent<DrawData>();
}

void Application_RegisterSystems()
{
    // Renderer Component Bitset consists of the following components:
    //  - DrawData
    //  - Transform
    ECS::ComponentBitset renderer_component_bitset;
    renderer_component_bitset.set(ECS::GetBitIndex<DrawData>());
    renderer_component_bitset.set(ECS::GetBitIndex<Transform>());
    g_ECSManager.RegisterSystem<Renderer>(renderer_component_bitset);
}

static void GetExtensions()
{
    std::filesystem::create_directories("cache/extensions");
    std::ofstream extensionOutFile("cache/extensions/extensions.txt", std::ios::trunc | std::ios::out);

    int extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensions);
    for (int i = 0; i < extensions; i++)
    {
        extensionOutFile << glGetStringi(GL_EXTENSIONS, i) << '\n';
    }
    extensionOutFile.close();
}

HNCRSP_NAMESPACE_END