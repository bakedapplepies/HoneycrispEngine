#include "Window.h"
#include "src/utils/TracyProfile.h"
#include "src/utils/Timer.h"
#include "src/managers/Texture2DManager.h"
#include "src/managers/ImGuiManager.h"

#include "src/scenes/DefaultScene.h"
#include "src/scenes/DefaultSceneTwo.h"
#include "src/scenes/SpaceScene.h"


HNCRSP_NAMESPACE_START

Window::Window(CallbackData* callbackData)
{
    m_glfwWindow = glfwGetCurrentContext();
    m_callbackData = callbackData;

    m_windowWidthScalar = callbackData->windowWidth / 1920.0f;  // TODO: Recalculate in callbacks
    m_windowHeightScalar = callbackData->windowHeight / 1080.0f;

    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)m_callbackData->windowWidth/(float)m_callbackData->windowHeight,
        0.1f,
        100.0f
    );

    // Add all post process shaders to m_pps
    _UpdatePPS();

    HNCRSP_LOG_INFO("Window Initialization done.");
}

void Window::Loop()
{
    if (!m_continueProgram) return;

    // ----- Systems -----
    Renderer* renderer = g_ECSManager.GetSystem<Renderer>();
    
    // ----- Setting up Camera -----
    m_camera = Camera(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_camera.position += glm::vec3(0, 10, 0);
    m_camera.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    renderer->SetCamera(&m_camera);

    // ----- Scenes -----
    uint32_t sceneOne = g_SceneManager.CreateScene<DefaultScene>();
    uint32_t sceneTwo = g_SceneManager.CreateScene<DefaultSceneTwo>();
    uint32_t spaceScene = g_SceneManager.CreateScene<SpaceScene>();
    g_SceneManager.SetActiveScene(sceneTwo);

    // ----- Window stuff -----
    float begin = glfwGetTime();

    // view matrix, ortho proj matrix, proj matrix, time
    UniformBuffer<glm::mat4, glm::mat4, glm::mat4, float> uboMatrices(0);  // UBO binding index
    // viewPos, spotlightPos, spotlightDir
    UniformBuffer<glm::vec3, glm::vec3, glm::vec3> uboOther(1);

    while(!glfwWindowShouldClose(m_glfwWindow))
    {
        ZoneScoped;
        m_deltaTime = glfwGetTime() - begin;
        begin = glfwGetTime();

        _CalcFPS();

        _ProcessInput();

        // Set window background color
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

        // ImGui
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui::NewFrame();

        // ----- Global settings -----
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_callbackData->windowWidth * 0.2f, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::Begin("Global settings");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;  // scale ui based on window height, might make it decay exponentially
        
        ImGui::SliderFloat("Camera Speed",
                            &m_camera.speed,
                            2.0f, 20.0f);

        static float depthPassCamDistFromMainCam = 50.0f;
        if (ImGui::SliderFloat("Depth-pass Camera's Distance from Main Camera",
                            &depthPassCamDistFromMainCam,
                            1.0f, 100.0f))
        {
            renderer->SetDepthPassCamDistFromMainCam(depthPassCamDistFromMainCam);
        }
        
        ImGui::NewLine();
        ImGui::SeparatorText("Post-processing effects variables");

        const Shader* postprocessingShader = g_ShaderManager.GetPostProcessingShader();
        static float     brightness   = 0.0f;
        static float     contrast     = 0.0f;  // -100.0 - 100.0
        static float     saturation   = 1.0f;  // -100.0 - 100.0
        static float     postExposure = 0.0f;
        static float     temperature  = 0.0f;
        static float     tint         = 0.0f;
        static float     cwhite       = 60.0f;
        static glm::vec3 colorFilter  = glm::vec3(1.0f);  // white
        
        if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 5.0f))
            postprocessingShader->SetFloatUnf("u_brightness", brightness);

        if (ImGui::SliderFloat("Contrast", &contrast, -100.0f, 100.0f))
            postprocessingShader->SetFloatUnf("u_contrast", contrast * 0.01f + 1.0f);

        if (ImGui::SliderFloat("Saturation", &saturation, 0.0f, 1.0f))
            postprocessingShader->SetFloatUnf("u_saturation", saturation);

        if (ImGui::DragFloat("Post Exposure", &postExposure))
            postprocessingShader->SetFloatUnf("u_postExposure", std::powf(2.0f, postExposure));

        if (ImGui::SliderFloat("Temperature", &temperature, 0.0f, 1.0f))
            postprocessingShader->SetFloatUnf("u_temperature", temperature);

        if (ImGui::SliderFloat("Tint", &tint, 0.0f, 1.0f))
            postprocessingShader->SetFloatUnf("u_tint", tint);

        if (ImGui::SliderFloat("CWhite", &cwhite, 1.0f, 120.0f))
            postprocessingShader->SetFloatUnf("u_cwhite", cwhite);

        if (ImGui::ColorPicker4("Color Filter", glm::value_ptr(colorFilter)))
            postprocessingShader->SetVec3Unf("u_colorFilter", colorFilter);

        ImGui::NewLine();
        ImGui::SeparatorText("Statistics");
        // float totalRenderTime = depthPass + scenePass + postprocessing;
        // ImGui::Text("Total Render time: %fms (%f%%)", totalRenderTime * 1000, totalRenderTime/m_deltaTime);

        // static bool anti_aliasing;
        // if (ImGui::Checkbox("Anti-aliasing", &anti_aliasing))
        // {
        //     HNCRSP_LOG_INFO(anti_aliasing);
        // }
        
        // ImGui::Image((void*)&Texture2DManager::mainTextureMap->getID(), ImVec2(100.0f, 100.0f));
        
        ImGui::End();

        // ----- Per-scene settings -----
        ImGui::SetNextWindowPos(ImVec2(0.0f, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_callbackData->windowWidth * 0.2f, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::Begin("Scene settings");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;

        // ImGui Windows from scenes
        g_SceneManager.UpdateImGui();

        ImGui::End();

        // ----- Scene Viewport -----
        ImGui::SetNextWindowPos(ImVec2(m_callbackData->windowWidth * 0.2f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_callbackData->windowWidth * 0.8f, m_callbackData->windowHeight), ImGuiCond_Always);
        ImGui::Begin("Viewport");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;

        GLuint colorBufferTextureID = renderer->GetColorBufferTextureID();
        ImGui::Image(
            reinterpret_cast<ImTextureID>(colorBufferTextureID),
            ImGui::GetContentRegionAvail(),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );

        ImGui::End();

        // Update camera
        m_camera.direction = glm::normalize(m_camera.direction);

        // ----- Global uniforms -----
        float u_time = begin;
        uboMatrices.Bind();
        uboMatrices.Update(
            glm::value_ptr(m_camera.GetViewMatrix()),
            glm::value_ptr(m_camera.GetOrthoMatrix(-5.0f, 5.0f, -5.0f, 5.0f)),
            glm::value_ptr(m_camera.GetProjectionMatrix(m_callbackData->windowWidth * 0.8f, m_callbackData->windowHeight)),
            &u_time
        );
        
        uboOther.Bind();
        uboOther.Update(
            glm::value_ptr(m_camera.position),
            glm::value_ptr(m_camera.position),
            glm::value_ptr(m_camera.direction)
        );

        // ----- Update and Render scenes -----
        g_SceneManager.Update(m_deltaTime);  // update scene before rendering
        renderer->Render();

        // ----- Render ImGui ----- always render after scene so it doesn't get drawn over
        g_ImGuiManager.Render();
                
        glfwSwapBuffers(m_glfwWindow);
        glfwPollEvents();

        FrameMark;
    }
}

void Window::_ProcessInput()
{
    ZoneScoped;

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_camera.ChangePos(glm::normalize(glm::vec3(m_camera.direction.x, 0.0f, m_camera.direction.z)) * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_camera.ChangePos(-glm::normalize(glm::vec3(m_camera.direction.x, 0.0f, m_camera.direction.z)) * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(m_camera.direction, m_camera.cameraUp));
        m_camera.ChangePos(m_camera.GetLeftVec() * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(m_camera.direction, m_camera.cameraUp));
        m_camera.ChangePos(m_camera.GetRightVec() * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_camera.ChangePos(glm::vec3(0.0f, 1.0f, 0.0f) * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_camera.ChangePos(glm::vec3(0.0f, -1.0f, 0.0f) * m_camera.speed * m_deltaTime);
    }
}

static uint32_t lowFPS  = 0xFFFFFFFF;
static uint32_t highFPS = 0;
static uint32_t countedFPSes = 0;
static uint32_t totalFPS = 0;  // uint32_t is more than enough
void Window::_CalcFPS()
{
    ZoneScoped;

    m_totalTime += m_deltaTime;
    m_frames++;
    if (m_totalTime >= 1.0f)
    {
        if (m_frames > highFPS) highFPS = m_frames;
        if (m_frames < lowFPS) lowFPS = m_frames;
        countedFPSes++;
        totalFPS += m_frames;

        std::string title = "Honeycrisp - FPS: " + fmt::to_string(m_frames);
        glfwSetWindowTitle(m_glfwWindow, title.c_str());
        m_frames = 0;
        m_totalTime = 0.0f;
    }
}

void Window::_UpdatePPS()
{
    ZoneScoped;

    static std::unordered_map<std::string, bool> umap;

    FileSystem::Path path("resources/shaders/postprocessing");
    for (const auto& entry : std::filesystem::directory_iterator(path.string()))
    {
        std::string str = entry.path().stem().string();
        if (str == "ScreenQuadVertex") continue;
        if (!umap[str])
        {
            m_pps.push_back(str);
            umap[str] = true;
        }
    }
}

Window::~Window()
{
    FileSystem::Path data_report_path(fmt::format("build/{}/src/data_report", HNCRSP_BUILD_TYPE));
    std::filesystem::create_directory(data_report_path.string());
    std::ofstream report_file(
        fmt::format("{}/fps_report.txt", data_report_path.string()), std::ios::out | std::ios::app);

    auto time_point = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(time_point);
    char* date = strtok(std::ctime(&time), "\n");

    report_file << fmt::format("[{}, {}] Low - High - Mean FPS: {} - {} - {}\n",
        HNCRSP_GIT_COMMIT_ID,
        date,
        lowFPS,
        highFPS,
        static_cast<float>(totalFPS) / static_cast<float>(countedFPSes)
    );
    report_file.close();
}

HNCRSP_NAMESPACE_END