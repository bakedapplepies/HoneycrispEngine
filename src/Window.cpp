#include "pch/pch.h"

#include "utils/Debug.h"
#include "Window.h"
#include "SceneManager.h"
#include "./scenes/DefaultScene.h"
#include "./scenes/DefaultSceneTwo.h"
#include "Callbacks.h"
#include "Model.h"


Window::Window()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create and assign OpenGL window context */
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    callbackData.windowWidth = mode->width * 0.75f;
    callbackData.windowHeight = mode->height * 0.75f;
    glfwWindow = glfwCreateWindow(
        callbackData.windowWidth,
        callbackData.windowHeight,
        "LearnOpenGL",
        nullptr,
        nullptr
    );

    if (glfwWindow == nullptr)
    {
        TERMINATE("GLFW Window Initialization failed.");
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);  // vsync


    /* Initialize GLAD -> Only call OpenGL functions after this */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        TERMINATE("GLAD Initialization failed.");
    }
    Debug::Log("OpenGL (Core) ", glGetString(GL_VERSION));


    /* Callbacks */
    glfwSetWindowUserPointer(glfwWindow, &callbackData);

    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(glfwWindow, mouse_callback);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwSetKeyCallback(glfwWindow, key_callback);

    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // disable cursor

    /* Depth, Stencil, Blending */
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glFrontFace(GL_CW));
    GLCall(glCullFace(GL_BACK));

    GLCall(glEnable(GL_STENCIL_TEST));

    // GLCall(glEnable(GL_BLEND));

    GLCall(glViewport(0, 0, callbackData.windowWidth, callbackData.windowHeight));


    /* Textures */
    stbi_set_flip_vertically_on_load(true);
    Texture2D::LoadTextures();

    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)callbackData.windowWidth/(float)callbackData.windowHeight,
        0.1f,
        100.0f
    );

    // ImGUI
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);

    ImGui::StyleColorsDark();

    Debug::Log("Window Initialization done.");
}


void Window::Loop()
{
    if (!continueProgram) return;

    SceneManager::Get().CreateScene(DefaultScene(), 0);
    SceneManager::Get().CreateScene(DefaultSceneTwo(), 1);
    SceneManager::Get().SetActiveScene(0);


    float begin = glfwGetTime();
    Textures::mainTextureMap.Bind();
    Textures::mainTextureSpecularMap.Bind();
    camera.SetPos(camera.cameraPos + glm::vec3(0, 10, 0));

    UniformBuffer<glm::mat4, glm::mat4, float> uboMatrices(0);  // binding index
    UniformBuffer<glm::vec3, glm::vec3, glm::vec3> uboOther(1);

    while(!glfwWindowShouldClose(glfwWindow))
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

        ImGui::Begin("Settings");
        static float lightSizeScale = 0.2f;
        static float waveSpeed = 1.0f;
        static float renderingTime = 0.0f;
        
        ImGui::SliderFloat("Light Size", &lightSizeScale, 0.0f, 1.0f);
        ImGui::SliderFloat("Wave Speed", &waveSpeed, 0.0f, 10.0f);
        ImGui::Text("Rendering time: %fms (%f%%)", renderingTime * 1000, renderingTime/deltaTime*100);
        ImGui::Text("Total time: %fms", deltaTime * 1000);

        ImGui::End();

        ImGui::Begin("Settings2");
        glm::vec3 lightColor = glm::vec3(0.0f);
        ImGui::SliderFloat3("Light Color", glm::value_ptr(lightColor), 0.0f, 1.0f);
        ImGui::End();

        // Update camera
        camera.SetDirection(glm::normalize(callbackData.cameraDirection));

        // TODO: put in callbackData
        projectionMatrix = glm::perspective(
            glm::radians(45.0f),
            (float)callbackData.windowWidth/(float)callbackData.windowHeight,
            0.1f,
            100.0f
        );

        // Global uniforms
        float u_time = begin*waveSpeed;
        uboMatrices.Update(
            glm::value_ptr(camera.GetViewMatrix()),
            glm::value_ptr(projectionMatrix),
            &u_time
        );
        
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
                
        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }
}


Window::~Window()
{
    ImGui::Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete these while OpenGL is still in context
    SceneManager::Get().ClearAllScenes();
    Texture2D::DeleteAllTextures();

    Debug::Log("Deallocated all resources.");
}


void Window::calcFPS()
{
    totalTime += deltaTime;
    frames++;
    if (totalTime >= 1.0f)
    {
        std::string title = "LearnOpenGL - FPS: " + fmt::to_string(frames);
        glfwSetWindowTitle(glfwWindow, title.c_str());
        frames = 0;
        totalTime = 0.0f;
    }
}

void Window::processInput()
{
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ChangePos(glm::normalize(glm::vec3(camera.direction.x, 0.0f, camera.direction.z)) * camera.speed * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ChangePos(-glm::normalize(glm::vec3(camera.direction.x, 0.0f, camera.direction.z)) * camera.speed * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.direction, camera.cameraUp));
        camera.ChangePos(-glm::vec3(direction.x, 0.0f, direction.z) * camera.speed * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.direction, camera.cameraUp));
        camera.ChangePos(glm::vec3(direction.x, 0.0f, direction.z) * camera.speed * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.ChangePos(glm::vec3(0.0f, 1.0f, 0.0f) * camera.speed * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ChangePos(glm::vec3(0.0f, -1.0f, 0.0f) * camera.speed * deltaTime);
    }
}
