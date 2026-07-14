#include "AppWindow.h"

#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>
#include <tracy/Tracy.hpp>

#include "core/Constants.h"
#include "util/Path.h"
#include "renderer/meshes/Quad.h"
#include "renderer/meshes/Model.h"
#include "renderer/PostprocessQueue.h"
// #include "renderer/ForwardRenderer.h"
#include "renderer/DeferredRenderer.h"

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
    m_GLFWUserData->hideCursor = true;
    glfwSetWindowUserPointer(m_window, m_GLFWUserData.get());

    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, MouseCallback);
    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSwapInterval(0);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fmt::println("Failed to initialize GLAD");
        return false;
    }

    m_applicationIsActive = true;
    m_envyInstance = &envy_instance;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init();
    
    return true;
}

void Application::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
    // Preload Shaders & Textures
    _LoadShaderPrograms();
    _LoadTexture2Ds();

    // Postprocess Queue
    PostprocessQueue postprocessQueue(m_envyInstance, 2560, 1440);
    postprocessQueue.AddCompute(m_envyInstance->GetShaderProgram(Path("engine/renderer/shaders/postprocess/gamma.comp").Str()));

    // Sub-systems setup
    m_renderer = std::make_unique<DeferredRenderer>(m_envyInstance);

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

    // Lights
    DirLight dirLight {
        .direction = glm::vec3(0.0, -1.0f, 0.0f)
    };
    std::vector<PointLight> pointLights;
    pointLights.push_back(PointLight {
        .position = glm::vec3(-1.0f, 2.0f, -1.0f)
    });
    pointLights.push_back(PointLight {
        .position = glm::vec3(1.0f, 1.0f, 1.0f)
    });

    // Camera
    Camera camera;
    m_mainCamera = &camera;
    m_GLFWUserData->cameraInControl = m_mainCamera;
    camera.position = glm::vec3(0.0f, 0.0f, 2.0f);
    camera.viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    Camera depthCamera;
    depthCamera.viewDir = glm::vec3(0.0f, -1.0f, 0.0f);
    depthCamera.position = camera.position - (depthCamera.viewDir) * 10.0f;

    // Global Material parameters
    MaterialUBO matParams {
        .u_ambient_intensity = 0.025f,
        .u_diffuse_intensity = 1.0f,
        .u_specular_intensity = 0.4f,
        .u_roughness_scalar = 0.4f
    };

    // Skybox
    const GLResource<Envy::Cubemap> skybox =
        m_envyInstance->CreateCubemap(Envy::TextureFormat::SRGBA8,
                                      TEXTURE_UNIT_CUBEMAP,
                                      Path("engine/resources/cubemaps/skybox/right.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/left.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/top.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/bottom.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/front.jpg").Str(),
                                      Path("engine/resources/cubemaps/skybox/back.jpg").Str());

    // Initialize UBOs
    m_renderer->UpdateDirLight(dirLight);
    m_renderer->UpdatePointLights(pointLights);
    m_renderer->UpdateGlobalMatParam(matParams);

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

        // ----- Shadow mapping -----
        FrameData shadowMapFrame {
            .frameType = FrameType::SHADOW,
            .framebuffer = m_renderer->GetMainShadowFramebuffer(),
            .fboClearBufferFlags = Envy::FBOBuffer::DEPTH,
            .camera = &depthCamera,
            .cameraProjection = CameraProjection::ORTHOGRAPHIC
        };
        m_renderer->BeginFrame(shadowMapFrame);
        m_renderer->RenderMultiple(model.GetRenderCmds());
        GLResource<Envy::Texture2D> shadowFrameResult = m_renderer->EndFrame();
        depthCamera.position = m_mainCamera->position;
        // --------------------------

        // ------- Main scene -------
        FrameData mainFrame {
            .frameType = FrameType::NORMAL,
            .framebuffer = m_renderer->GetMainFramebuffer(),
            .fboClearBufferFlags = Envy::FBOBuffer::COLOR | Envy::FBOBuffer::DEPTH,
            .camera = m_mainCamera,
            .cameraProjection = CameraProjection::PERSPECTIVE
        };
        m_renderer->BeginFrame(mainFrame);
        shadowFrameResult->Bind(TEXTURE_UNIT_DEPTH_MAP);
        m_renderer->RenderMultiple(model.GetRenderCmds());
        GLResource<Envy::Texture2D> mainFrameResult = m_renderer->EndFrame(skybox);
        // --------------------------
        depthCamera.position = camera.position - (depthCamera.viewDir) * 20.0f;

        m_envyInstance->BindDefaultFramebuffer();

        GLResource<Envy::Texture2D> frame = postprocessQueue.Process(mainFrameResult, 320, 180, 1);
        static ImTextureID textureID = frame->GetID();

        _ProcessInput(deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        // ImGui::ShowDemoWindow();

        // ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
        ImGui::Begin("Settings");
        ImGui::GetCurrentWindow()->FontWindowScale = 2.0f;
        bool updateDepthMapDir = false;
        bool updateDirLight = false;
        bool updatePointLight = false;
        bool updateGlobalMatParams = false;
        static glm::vec3 dirLightDir = dirLight.direction;
        updateDirLight |= ImGui::SliderFloat3("Directional Light", glm::value_ptr(dirLightDir), -10.0f, 10.0f);
        if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS)
        {
            textureID = frame->GetID();
        }
        else if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
        {
            // textureID = shadowFrameResult->GetID();
        }
        updateDepthMapDir |= ImGui::Button("Set Dir");
        updatePointLight |= ImGui::SliderFloat3("PointLight 1", glm::value_ptr(pointLights[0].position), -10.0f, 10.0f);
        updatePointLight |= ImGui::SliderFloat3("PointLight 2", glm::value_ptr(pointLights[1].position), -10.0f, 10.0f);
        updateGlobalMatParams |= ImGui::SliderFloat("Ambient Intensity", &matParams.u_ambient_intensity, 0.0f, 1.0f);
        updateGlobalMatParams |= ImGui::SliderFloat("Diffuse Intensity", &matParams.u_diffuse_intensity, 0.0f, 1.0f);
        updateGlobalMatParams |= ImGui::SliderFloat("Specular Intensity", &matParams.u_specular_intensity, 0.0f, 1.0f);
        updateGlobalMatParams |= ImGui::SliderFloat("Roughness Scalar", &matParams.u_roughness_scalar, 0.0f, 1.0f);
        // if (updateDepthMapDir) depthCamera.viewDir = -m_mainCamera->viewDir;
        if (updateDirLight)
        {
            dirLight.direction = glm::normalize(dirLightDir);
            depthCamera.viewDir = dirLight.direction;
            m_renderer->UpdateDirLight(dirLight);
        } 
        if (updatePointLight) m_renderer->UpdatePointLights(pointLights);
        if (updateGlobalMatParams) m_renderer->UpdateGlobalMatParam(matParams);
        ImGui::End();

        // ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
        ImGui::Begin("Viewport");
        ImGui::GetCurrentWindow()->FontWindowScale = 2.0f;
        float viewportX, viewportY;
        ImVec2 imageDimension = ImGui::GetContentRegionAvail();
        if (imageDimension.x/imageDimension.y > 16.0f/9.0f)  // avoid image stretching
        {
            viewportY = imageDimension.y;
            viewportX = viewportY * 16.0f / 9.0f;
        }
        else
        {
            viewportX = imageDimension.x;
            viewportY = viewportX * 9.0f / 16.0f;
        }
        // TODO: Place viewport image in the middle of window/viewport tab
        ImGui::Image(textureID,
                     ImVec2(viewportX, viewportY),
                     ImVec2(0.0f, 1.0f),
                     ImVec2(1.0f, 0.0f));
        ImGui::End();

        m_envyInstance->ClearBuffer();
        m_renderer->GetMainFramebuffer()->CopyToDefaultFBO(2560, 1440, Envy::FBOBuffer::COLOR);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
        FrameMark;

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
    {
        m_mainCamera->MoveForward(4.0f, delta_time);
    }

    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_mainCamera->MoveBackward(4.0f, delta_time);
    }

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_mainCamera->MoveLeft(4.0f, delta_time);
    }

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_mainCamera->MoveRight(4.0f, delta_time);
    }

    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_mainCamera->MoveUp(4.0f, delta_time);
    }

    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_mainCamera->MoveDown(4.0f, delta_time);
    }
}

void Application::_LoadShaderPrograms() const
{
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::VERTEX,
        Path("engine/renderer/shaders/default.vert").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::FRAGMENT,
        Path("engine/renderer/shaders/forward.frag").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::FRAGMENT,
        Path("engine/renderer/shaders/deferred.frag").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::VERTEX,
        Path("engine/renderer/shaders/screen_quad.vert").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::FRAGMENT,
        Path("engine/renderer/shaders/deferred_shading.frag").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::VERTEX,
        Path("engine/renderer/shaders/shadow.vert").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::FRAGMENT,
        Path("engine/renderer/shaders/shadow.frag").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::COMPUTE,
        Path("engine/renderer/shaders/postprocess/gamma.comp").Str());
}

void Application::_LoadTexture2Ds() const
{
    m_envyInstance->LoadTexture2D(
        Envy::TextureFormat::SRGBA8,
        Path("engine/resources/images/villager.png").Str());
}

HNCRSP_NAMESPACE_END