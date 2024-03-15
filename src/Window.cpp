#include "src/pch/pch.h"
#include "src/core/Texture2DManager.h"

#include "Window.h"
#include "src/core/SceneManager.h"
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
    camera.SetPos(camera.cameraPos + glm::vec3(0, 10, 0));

    // view matrix, proj matrix, time
    UniformBuffer<glm::mat4, glm::mat4, float> uboMatrices(0);  // binding index
    // viewPos, spotlightPos, spotlightDir
    UniformBuffer<glm::vec3, glm::vec3, glm::vec3> uboOther(1);

    Renderer* renderer = g_ECSManager->GetSystem<Renderer>().get();

    while(!glfwWindowShouldClose(m_glfwWindow))
    {
        m_deltaTime = glfwGetTime() - begin;
        begin = glfwGetTime();

        calcFPS();

        processInput();

        // Set window background color
        glm::vec3 bgColor = g_SceneManager.GetSceneBgColor();
        GLCall(glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f));

        // Clear buffers
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        // ImGui
        m_callbackData->settingsWidth = m_callbackData->settingsWidthPercentage * m_callbackData->windowWidth;

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_callbackData->settingsWidth, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::Begin("Global settings");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;  // scale the ui based on window height
        
        static float waveSpeed = 1.0f;
        ImGui::SliderFloat("Wave Speed", &waveSpeed, 0.0f, 10.0f);

        static float renderingTime = 0.0f;
        ImGui::Text("Rendering time: %fms (%f%%)", renderingTime * 1000, renderingTime/m_deltaTime*100);
        ImGui::Text("Total time: %fms", m_deltaTime * 1000);

        // ImGui::Image((void*)&Texture2DManager::mainTextureMap->getID(), ImVec2(100.0f, 100.0f));
        
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0.0f, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_callbackData->settingsWidth, m_callbackData->windowHeight * 0.5f), ImGuiCond_Always);
        ImGui::Begin("Scene settings");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;

        g_SceneManager.UpdateImGui();

        ImGui::End();

        // Update camera
        camera.SetDirection(glm::normalize(m_callbackData->cameraDirection));

        projectionMatrix = glm::perspective(
            glm::radians(45.0f),
            (float)(m_callbackData->windowWidth - 500.0f)/(float)m_callbackData->windowHeight,
            0.1f,
            100.0f
        );

        // Global uniforms
        float u_time = begin * waveSpeed;
        uboMatrices.Bind();
        uboMatrices.Update(
            glm::value_ptr(camera.GetViewMatrix()),
            glm::value_ptr(projectionMatrix),
            &u_time
        );
        
        uboOther.Bind();
        uboOther.Update(
            glm::value_ptr(camera.cameraPos),
            glm::value_ptr(camera.cameraPos),
            glm::value_ptr(camera.direction)
        );

        g_SceneManager.Update(m_deltaTime);  // TODO: Move cubemap rendering to Renderer
        renderingTime = glfwGetTime();
        g_ECSManager->Update();
        renderingTime = glfwGetTime() - renderingTime;

        ImGui::Render();  // always render after scene so it doesn't get drawn over
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                
        glfwSwapBuffers(m_glfwWindow);
        glfwPollEvents();
    }
}

void Window::processInput()
{
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

Window::~Window()
{
    std::filesystem::create_directory("data_report");
    std::ofstream report_file("data_report/fps_report.txt", std::ios::out | std::ios::trunc);

    report_file << "Low FPS: " << lowFPS << '\n';
    report_file << "High FPS: " << highFPS;
    report_file.close();
}

HNCRSP_NAMESPACE_END