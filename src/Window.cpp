#include "src/pch/pch.h"
#include "src/core/Texture2DManager.h"

#include "Window.h"
#include "src/core/SceneManager.h"
#include "src/scenes/DefaultScene.h"
#include "src/scenes/DefaultSceneTwo.h"


HNCRSP_NAMESPACE_START

void Window::StartUp(GLFWwindow* glfwWindow, RenderContext::CallbackData* callbackData)
{
    m_glfwWindow = glfwWindow;
    m_callbackData = callbackData;

    m_windowWidth = callbackData->windowWidth;
    m_windowHeight = callbackData->windowHeight;
    m_windowWidthScalar = callbackData->windowWidth / 1920.0f;
    m_windowHeightScalar = callbackData->windowHeight / 1080.0f;
    HNCRSP_LOG_INFO(m_windowHeightScalar);

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

    SceneManager::Get().CreateScene(DefaultScene(), 0);
    SceneManager::Get().CreateScene(DefaultSceneTwo(), 1);
    SceneManager::Get().SetActiveScene(0);


    float begin = glfwGetTime();
    Texture2DManager::mainTextureMap->Bind();
    Texture2DManager::mainTextureSpecularMap->Bind();
    camera.SetPos(camera.cameraPos + glm::vec3(0, 10, 0));

    // view matrix, proj matrix, time
    UniformBuffer<glm::mat4, glm::mat4, float> uboMatrices(0);  // binding index
    // viewPos, spotlightPos, spotlightDir
    UniformBuffer<glm::vec3, glm::vec3, glm::vec3> uboOther(1);

    while(!glfwWindowShouldClose(m_glfwWindow))
    {
        m_deltaTime = glfwGetTime() - begin;
        begin = glfwGetTime();

        calcFPS();

        processInput();

        // Set background
        glm::vec3 bgColor = SceneManager::Get().GetSceneBgColor();
        GLCall(glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f));

        // Clear buffers
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        // ImGui
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(m_windowWidth * (1.0f - m_callbackData->viewportWidthPercentage), m_windowHeight * 0.5f), ImGuiCond_Once);
        ImGui::Begin("Global settings");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;
        
        static float waveSpeed = 1.0f;
        ImGui::SliderFloat("Wave Speed", &waveSpeed, 0.0f, 10.0f);

        static float renderingTime = 0.0f;
        ImGui::Text("Rendering time: %fms (%f%%)", renderingTime * 1000, renderingTime/m_deltaTime*100);
        ImGui::Text("Total time: %fms", m_deltaTime * 1000);

        // ImGui::Image((void*)&Texture2DManager::mainTextureMap->getID(), ImVec2(100.0f, 100.0f));
        
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0.0f, m_windowHeight * 0.5f), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(m_windowWidth * (1.0f - m_callbackData->viewportWidthPercentage), m_windowHeight * 0.5f), ImGuiCond_Once);
        ImGui::Begin("Scene settings");
        ImGui::GetCurrentWindow()->FontWindowScale = m_windowHeightScalar;

        SceneManager::Get().UpdateImGui();

        ImGui::End();

        // Update camera
        camera.SetDirection(glm::normalize(m_callbackData->cameraDirection));

        // TODO: put in m_callbackData
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

        renderingTime = glfwGetTime();
        SceneManager::Get().Update();
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

void Window::calcFPS()
{
    m_totalTime += m_deltaTime;
    m_frames++;
    if (m_totalTime >= 1.0f)
    {
        std::string title = "LearnOpenGL - FPS: " + fmt::to_string(m_frames);
        glfwSetWindowTitle(m_glfwWindow, title.c_str());
        m_frames = 0;
        m_totalTime = 0.0f;
    }
}

HNCRSP_NAMESPACE_END