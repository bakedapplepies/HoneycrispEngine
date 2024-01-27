#include "src/pch/pch.h"

#include "Window.h"
#include "src/core/SceneManager.h"
#include "src/scenes/DefaultScene.h"
#include "src/scenes/DefaultSceneTwo.h"
#include "Callbacks.h"


HNCRSP_NAMESPACE_START

void Window::StartUp(GLFWwindow* glfwWindow, RenderContext::CallbackData* callbackData)
{
    m_glfwWindow = glfwWindow;
    m_callbackData = callbackData;

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
    if (!continueProgram) return;

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
        deltaTime = glfwGetTime() - begin;
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

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(100.0f, 100.0f));
        ImGui::Begin("Settings");
        static float lightSizeScale = 0.2f;
        static float waveSpeed = 1.0f;
        static float renderingTime = 0.0f;
        
        ImGui::SliderFloat("Light Size", &lightSizeScale, 0.0f, 1.0f);
        ImGui::SliderFloat("Wave Speed", &waveSpeed, 0.0f, 10.0f);
        ImGui::Text("Rendering time: %fms (%f%%)", renderingTime * 1000, renderingTime/deltaTime*100);
        ImGui::Text("Total time: %fms", deltaTime * 1000);

        static glm::vec3 lightColor = glm::vec3(0.0f);
        static glm::vec3 lightDir = glm::vec3(1.0f);
        ImGui::SliderFloat3("Light Color", glm::value_ptr(lightColor), 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Dir", glm::value_ptr(lightDir), 0.0f, 1.0f);
        ImGui::End();

        // Update camera
        camera.SetDirection(glm::normalize(m_callbackData->cameraDirection));

        // TODO: put in m_callbackData
        projectionMatrix = glm::perspective(
            glm::radians(45.0f),
            (float)m_callbackData->windowWidth/(float)m_callbackData->windowHeight,
            0.1f,
            100.0f
        );

        // Global uniforms
        float u_time = begin*waveSpeed;
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

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                
        glfwSwapBuffers(m_glfwWindow);
        glfwPollEvents();
    }
}

void Window::processInput()
{
    if (glfwGetKey(m_glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ChangePos(glm::normalize(glm::vec3(camera.direction.x, 0.0f, camera.direction.z)) * camera.speed * deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ChangePos(-glm::normalize(glm::vec3(camera.direction.x, 0.0f, camera.direction.z)) * camera.speed * deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.direction, camera.cameraUp));
        camera.ChangePos(-glm::vec3(direction.x, 0.0f, direction.z) * camera.speed * deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.direction, camera.cameraUp));
        camera.ChangePos(glm::vec3(direction.x, 0.0f, direction.z) * camera.speed * deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.ChangePos(glm::vec3(0.0f, 1.0f, 0.0f) * camera.speed * deltaTime);
    }

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ChangePos(glm::vec3(0.0f, -1.0f, 0.0f) * camera.speed * deltaTime);
    }
}

void Window::calcFPS()
{
    totalTime += deltaTime;
    frames++;
    if (totalTime >= 1.0f)
    {
        std::string title = "LearnOpenGL - FPS: " + fmt::to_string(frames);
        glfwSetWindowTitle(m_glfwWindow, title.c_str());
        frames = 0;
        totalTime = 0.0f;
    }
}

HNCRSP_NAMESPACE_END