#include "Application.h"

// Managers
#include "src/WindowHandler.h"
#include "src/RenderContext.h"
#include "api/managers/ImGuiManager.h"
#include "api/managers/Texture2DManager.h"
#include "api/managers/ShaderManager.h"
#include "src/managers/SceneManager.h"
#include "src/Callbacks.h"
#include "src/GameLoop.h"
#include "Client/src/GameEntry.h"
#include "api/ecs/ECSManager.h"

// Components
#include "api/components/Transform.h"
#include "api/components/DrawData.h"

// ECS Systems
#include "api/systems/Renderer.h"


HNCRSP_NAMESPACE_START

static void GetExtensions();
void Application::Run()
{
    WindowHandler windowHandler;
    windowHandler.StartUp();

    RenderContext renderContext;
    renderContext.Initialize();

    GetExtensions();

    uint64_t ecsManagerKey = g_ECSManager.StartUp();
    Application_RegisterComponents();
    Application_RegisterSystems();
    uint64_t imGuiManagerKey = g_ImGuiManager.StartUp();
    uint64_t shaderManagerKey = g_ShaderManager.StartUp();
    uint64_t texture2DManagerKey = g_Texture2DManager.StartUp();
    uint64_t sceneManagerKey = g_SceneManager.StartUp();
    // Application_RegisterScenes();

    // resources in Window class are mostly managed by other managers
    // post-deletion of resources is mostly trivial
    GameLoop gameLoop;
    GameEntry gameEntry;
    gameEntry.RegisterScenes();
    gameLoop.Start();

    g_SceneManager.ShutDown(sceneManagerKey);
    g_ECSManager.ShutDown(ecsManagerKey);
    g_Texture2DManager.ShutDown(texture2DManagerKey);
    g_ShaderManager.ShutDown(shaderManagerKey);
    g_ImGuiManager.ShutDown(imGuiManagerKey);
    windowHandler.ShutDown();
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

// void Application_RegisterScenes()
// {
//     uint32_t sceneOne = g_SceneManager.CreateScene<DefaultScene>();
//     uint32_t sceneTwo = g_SceneManager.CreateScene<DefaultSceneTwo>();
//     uint32_t spaceScene = g_SceneManager.CreateScene<SpaceScene>();

//     // Load this scene first
//     g_SceneManager.SetActiveScene(sceneTwo);
// }

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