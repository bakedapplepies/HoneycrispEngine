#include "Application.h"

// Managers
#include "src/managers/WindowHandler.h"
#include "src/managers/RenderContext.h"
#include "src/managers/ImGuiManager.h"
#include "src/managers/Texture2DManager.h"
#include "src/managers/ShaderManager.h"
#include "src/managers/SceneManager.h"
#include "src/Callbacks.h"
#include "src/GameLoop.h"
#include "src/ecs/ECSManager.h"

// Components
#include "src/components/Transform.h"
#include "src/components/DrawData.h"

// ECS Systems
#include "src/systems/Renderer.h"

// Scenes
#include "src/scenes/DefaultScene.h"
#include "src/scenes/DefaultSceneTwo.h"
#include "src/scenes/SpaceScene.h"


HNCRSP_NAMESPACE_START

static void GetExtensions();
void Application::Run()  // this is where the main control flow happens
{
    // std::filesystem::create_directory("error");
    // std::ofstream stderr_redirect("error/error.txt", std::ios::out | std::ios::trunc);
    // std::streambuf* cerrBuf = std::cerr.rdbuf();  // save cerr buffer
    // std::cerr.rdbuf(stderr_redirect.rdbuf());  // redirect to file buffer

    WindowHandler windowHandler;
    windowHandler.StartUp();

    RenderContext renderContext;
    renderContext.StartUp();

    GetExtensions();

    g_ECSManager.StartUp();
    Application_RegisterComponents();
    Application_RegisterSystems();
    g_ImGuiManager.StartUp();
    g_ShaderManager.StartUp();
    g_Texture2DManager.StartUp();
    g_SceneManager.StartUp();
    Application_RegisterScenes();

    // resources in Window class are mostly managed by other managers
    // post-deletion of resources is mostly trivial
    GameLoop gameLoop;
    gameLoop.Start();

    g_SceneManager.ClearAllScenes();
    g_SceneManager.ShutDown();
    g_ECSManager.ShutDown();
    g_Texture2DManager.ShutDown();
    g_ShaderManager.ShutDown();
    g_ImGuiManager.ShutDown();
    windowHandler.ShutDown();

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

void Application_RegisterScenes()
{
    uint32_t sceneOne = g_SceneManager.CreateScene<DefaultScene>();
    uint32_t sceneTwo = g_SceneManager.CreateScene<DefaultSceneTwo>();
    uint32_t spaceScene = g_SceneManager.CreateScene<SpaceScene>();

    // Load this scene first
    g_SceneManager.SetActiveScene(sceneTwo);
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