#include "Window.h"
#include "src/managers/Texture2DManager.h"
#include "src/managers/ImGuiManager.h"

#include "src/scenes/DefaultScene.h"
#include "src/scenes/DefaultSceneTwo.h"
#include "src/scenes/SpaceScene.h"


HNCRSP_NAMESPACE_START

Window::Window(RenderContext::CallbackData* callbackData)
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

    updatePPS();

    HNCRSP_LOG_INFO("Window Initialization done.");
}

void Window::Loop()
{
    if (!m_continueProgram) return;

    size_t scene_one = g_SceneManager.CreateScene<DefaultScene>();
    size_t scene_two = g_SceneManager.CreateScene<DefaultSceneTwo>();
    size_t space_scene = g_SceneManager.CreateScene<SpaceScene>();
    g_SceneManager.SetActiveScene(scene_one);

    float begin = glfwGetTime();
    g_Texture2DManager.mainTextureMap->Bind();
    g_Texture2DManager.mainTextureSpecularMap->Bind();

    Camera& camera = m_callbackData->camera;
    camera.SetPos(camera.position + glm::vec3(0, 10, 0));

    // view matrix, proj matrix, time
    UniformBuffer<glm::mat4, glm::mat4, float> uboMatrices(0);  // UBO binding index
    // viewPos, spotlightPos, spotlightDir
    UniformBuffer<glm::vec3, glm::vec3, glm::vec3> uboOther(1);
    g_ShaderManager.SetPostProcessingShader(FileSystem::Path("resources/shaders/postprocessing/None.glsl"));

    while(!glfwWindowShouldClose(m_glfwWindow))
    {
        m_deltaTime = glfwGetTime() - begin;
        begin = glfwGetTime();

        calcFPS();

        processInput();

        // Set window background color
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

        // ImGui
        m_callbackData->settingsWidth = m_callbackData->settingsWidthPercentage * m_callbackData->windowWidth;

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui::NewFrame();

        // Global settings ----------
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_callbackData->settingsWidth, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::Begin("Global settings");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;  // scale ui based on window height, might make it decay exponentially
        
        static float waveSpeed = 1.0f;
        ImGui::SliderFloat("Wave Speed", &waveSpeed, 0.0f, 10.0f);

        ImGui::SliderFloat("Camera Speed", &camera.speed, 2.0f, 20.0f);

        ImGui::SeparatorText("Post-processors:");
        ImGui::Text(
            "Files that are added in \"resources/shaders/postprocessing/\" during\n"
            "runtime can be updated using the button below."
        );
        ImGui::NewLine();

        static uint32_t current_postprocessor_index = 0xFFFFFFFF;

        if (ImGui::Button("Update postprocessing shader list"))
        {
            updatePPS();
        }

        ImGui::NewLine();
        ImGui::Text("Added Post-processing shaders:");
        if (ImGui::BeginListBox(" "))
        {
            for (uint32_t i = 0; i < m_pps.size(); i++)
            {
                const bool is_selected = (i == current_postprocessor_index);
                if (ImGui::Selectable(m_pps[i].c_str(), is_selected))
                {
                    g_ShaderManager.SetPostProcessingShader(
                        FileSystem::Path(fmt::format("resources/shaders/postprocessing/{}.glsl", m_pps[i]))
                    );
                    current_postprocessor_index = i;
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }

        ImGui::NewLine();
        ImGui::SeparatorText("Statistics");
        static float renderingTime = 0.0f;
        ImGui::Text("Rendering time: %fms (%f%%)", renderingTime * 1000, renderingTime/m_deltaTime*100);
        ImGui::Text("Total time: %fms", m_deltaTime * 1000);

        // static bool anti_aliasing;
        // if (ImGui::Checkbox("Anti-aliasing", &anti_aliasing))
        // {
        //     HNCRSP_LOG_INFO(anti_aliasing);
        // }
        
        // ImGui::Image((void*)&Texture2DManager::mainTextureMap->getID(), ImVec2(100.0f, 100.0f));
        
        ImGui::End();

        // Per-scene settings ----------
        ImGui::SetNextWindowPos(ImVec2(0.0f, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_callbackData->settingsWidth, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::Begin("Scene settings");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;

        // ImGui Windows from scenes
        g_SceneManager.UpdateImGui();

        ImGui::End();

        // Update camera
        camera.SetDirection(glm::normalize(camera.direction));

        // FOV maybe added later, not important
        projectionMatrix = glm::perspective(
            glm::radians(45.0f),
            (float)(m_callbackData->windowWidth - 500.0f)/(float)m_callbackData->windowHeight,
            0.1f,
            100.0f
        );

        // Global uniforms ----------
        float u_time = begin * waveSpeed;
        uboMatrices.Bind();
        uboMatrices.Update(
            glm::value_ptr(camera.GetViewMatrix()),
            glm::value_ptr(projectionMatrix),
            &u_time
        );
        
        uboOther.Bind();
        uboOther.Update(
            glm::value_ptr(camera.position),
            glm::value_ptr(camera.position),
            glm::value_ptr(camera.direction)
        );

        // Update and Render scenes ----------
        g_SceneManager.Update(m_deltaTime);  // update scene before rendering
        renderingTime = glfwGetTime();
        g_ECSManager->Update();
        renderingTime = glfwGetTime() - renderingTime;

        // Render ImGui ---------- always render after scene so it doesn't get drawn over
        g_ImGuiManager.Render();
                
        glfwSwapBuffers(m_glfwWindow);
        glfwPollEvents();
    }
}

void Window::processInput()
{
    Camera& camera = m_callbackData->camera;
    if (glfwGetKey(m_glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ChangePos(glm::normalize(glm::vec3(camera.direction.x, 0.0f, camera.direction.z)) * camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ChangePos(-glm::normalize(glm::vec3(camera.direction.x, 0.0f, camera.direction.z)) * camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.direction, camera.cameraUp));
        camera.ChangePos(-glm::vec3(direction.x, 0.0f, direction.z) * camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.direction, camera.cameraUp));
        camera.ChangePos(glm::vec3(direction.x, 0.0f, direction.z) * camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.ChangePos(glm::vec3(0.0f, 1.0f, 0.0f) * camera.speed * m_deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ChangePos(glm::vec3(0.0f, -1.0f, 0.0f) * camera.speed * m_deltaTime);
    }
}

static unsigned int lowFPS  = 0xFFFFFFFF;
static unsigned int highFPS = 0;
void Window::calcFPS()
{
    m_totalTime += m_deltaTime;
    m_frames++;
    if (m_totalTime >= 1.0f)
    {
        if (m_frames > highFPS) highFPS = m_frames;
        if (m_frames < lowFPS) lowFPS = m_frames;
        std::string title = "Honeycrisp - FPS: " + fmt::to_string(m_frames);
        glfwSetWindowTitle(m_glfwWindow, title.c_str());
        m_frames = 0;
        m_totalTime = 0.0f;
    }
}

void Window::updatePPS()
{
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

    report_file << fmt::format("[{}, {}] Low - High FPS: {} - {}\n", HNCRSP_GIT_COMMIT_ID, date, lowFPS, highFPS);
    report_file.close();
}

HNCRSP_NAMESPACE_END