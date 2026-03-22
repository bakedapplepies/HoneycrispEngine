#include "AppWindow.h"

#include <fmt/format.h>

#include "util/Path.h"
#include "meshes/Quad.h"
#include "meshes/Model.h"

HNCRSP_NAMESPACE_START

Application::Application()
{}

Application::~Application()
{}

bool Application::Create(const WindowConfig& window_config, const Envy::EnvyInstance& envy_instance)
{
    // TODO: Add Wayland checks
    glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);

    /* Initialize the library */
    if (!glfwInit())
        return false;

    glfwSetErrorCallback(ErrorCallback);

    // TODO: Add WindowConfig option to specify vulkan or opengl
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    m_window = glfwCreateWindow(window_config.width,
                                window_config.height,
                                window_config.name,
                                NULL,
                                NULL);
    if (!m_window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    m_GLFWUserData = std::make_unique<GLFWUserData>();
    glfwSetWindowUserPointer(m_window, m_GLFWUserData.get());
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, MouseCallback);
    glfwSwapInterval(0);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fmt::println("Failed to initialize GLAD");
        return false;
    }

    m_applicationIsActive = true;
    m_envyInstance = &envy_instance;
    
    return true;
}

void Application::Destroy()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();

    m_window = nullptr;
    m_GLFWUserData.reset();
    m_applicationIsActive = false;

    m_renderer.reset();
    m_mainCamera = nullptr;
}

void Application::Run()
{
    // Sub-systems setup
    m_renderer = std::make_unique<Renderer>(m_envyInstance);

    _LoadShaderPrograms();
    _LoadTexture2Ds();

    // Scene data
    std::vector<glm::vec3> quadPositions;
    quadPositions.resize(10000);
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            quadPositions[100 * i + j] = glm::vec3(2.0f * i, 0.0f, -2.0f * j);
        }
    }

    Quad quad(m_envyInstance);

    RenderCommand quadRenderCmd = quad.GetRenderCmd();
    quadRenderCmd.vertexArray->AddInstanceBuffer(3, sizeof(glm::vec3), 10000, quadPositions.data());
    Envy::IndirectCommand indirectCmd {
        .count = quadRenderCmd.vaoChunk->elementsCount,
        .instanceCount = 10000,
        .firstIndex = quadRenderCmd.vaoChunk->elementsOffset,
        .baseVertex = quadRenderCmd.vaoChunk->vertexOffset,
        .baseInstance = 0
    };
    GLResource<Envy::IndirectBuffer> quadIndirectBuffer =
        m_envyInstance->CreateIndirectBuffer(1, &indirectCmd);
    RenderCommandIndirect quadRenderCmdIndirect {
        .vertexArray = quadRenderCmd.vertexArray,
        .indirectBuffer = quadIndirectBuffer,
        .material = quadRenderCmd.material,
        .transform = quadRenderCmd.transform
    };

    Model model(Path("engine/resources/models/sponza2/Sponza.gltf").Str(), m_envyInstance);

    // Camera
    Camera camera;
    m_mainCamera = &camera;
    m_GLFWUserData->cameraInControl = m_mainCamera;
    camera.position = glm::vec3(0.0f, 0.0f, 2.0f);
    camera.viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    // Skybox
    const GLResource<Envy::Cubemap> skybox =
        m_envyInstance->CreateCubemap(Envy::TextureFormat::SRGBA8,
                                      Path("engine/resources/cubemaps/skybox/right.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/left.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/top.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/bottom.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/front.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/back.jpg").Str());

    // Keep track of time             
    float beginTime = glfwGetTime();
    float totalTime = 0.0f;
    float deltaTime = 0.0f;
    uint32_t totalFrames = 0;
    
    while (m_applicationIsActive)
    {
        deltaTime = glfwGetTime() - beginTime;
        totalTime += deltaTime;
        beginTime = glfwGetTime();

        // Main logic

        m_renderer->BeginFrame(m_mainCamera);
        // m_renderer->RenderIndirect(quadRenderCmdIndirect);
        m_renderer->RenderMultiple(model.GetRenderCmds());
        m_renderer->EndFrame(skybox);
        glfwSwapBuffers(m_window);

        _ProcessInput(deltaTime);

        //

        if (totalTime >= 1.0f)
        {       
            totalTime = 0.0f;
            fmt::println("FPS: {}", totalFrames);
            totalFrames = 0;
        }
        totalFrames++;
    }
}

void Application::_ProcessInput(float delta_time)
{
    glfwPollEvents();
    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
        m_applicationIsActive = false;


    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_mainCamera->MoveForward(4.0f, delta_time);

    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_mainCamera->MoveBackward(4.0f, delta_time);

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_mainCamera->MoveLeft(4.0f, delta_time);

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_mainCamera->MoveRight(4.0f, delta_time);

    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_mainCamera->MoveUp(4.0f, delta_time);

    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_mainCamera->MoveDown(4.0f, delta_time);
}

void Application::_LoadShaderPrograms() const
{
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::VERTEX,
        Path("engine/renderer/shaders/default.vert").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::FRAGMENT,
        Path("engine/renderer/shaders/default.frag").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::VERTEX,
        Path("engine/renderer/shaders/screen_quad.vert").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::FRAGMENT,
        Path("engine/renderer/shaders/screen_quad.frag").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::COMPUTE,
        Path("engine/renderer/shaders/default.comp").Str());
}

void Application::_LoadTexture2Ds() const
{
    m_envyInstance->LoadTexture2D(
        Envy::TextureFormat::SRGBA8,
        Path("engine/resources/images/villager.png").Str());
}

HNCRSP_NAMESPACE_END