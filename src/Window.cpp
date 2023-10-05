#include "pch/pch.h"

#include "Debug.h"
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
        NULL,
        NULL
    );

    if (glfwWindow == NULL)
    {
        Debug::Error("GLFW Window Initialization failed.");
        glfwTerminate();
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);  // vsync


    /* Initialize GLAD -> Only call OpenGL functions after this */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Debug::Error("GLAD Initialization failed.");
        glfwTerminate();
    }
    Debug::Log("OpenGL (Core) ", glGetString(GL_VERSION));


    /* Callbacks */
    glfwSetWindowUserPointer(glfwWindow, &callbackData);

    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(glfwWindow, mouse_callback);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwSetKeyCallback(glfwWindow, key_callback);

    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // disable cursor

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glFrontFace(GL_CW));
    GLCall(glCullFace(GL_BACK));
    // GLCall(glEnable(GL_BLEND));
    GLCall(glViewport(0, 0, callbackData.windowWidth, callbackData.windowHeight));

    Debug::Log("Window config done.");

    /* Textures */
    stbi_set_flip_vertically_on_load(true);
    Texture::LoadTextures();

    viewMatrix = camera.GetViewMatrix();
    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)callbackData.windowWidth/(float)callbackData.windowHeight,
        0.1f,
        100.0f
    );

    light = Light(
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    light.AddPosition(glm::vec3(1.0f, 1.0f, 3.0f));

    // Model backpack("../resources/models/backpack/backpack.obj");

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

    GLuint ubo[2];
    GLCall(glGenBuffers(2, ubo));

    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo[0]));
    GLCall(glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(GLfloat), nullptr, GL_STATIC_DRAW));
    GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo[0], 0, 2 * sizeof(glm::mat4) + sizeof(float)));

    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo[1]));
    GLCall(glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW));
    GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 1, ubo[1], 0, 2 * sizeof(glm::vec3)));

    while(!glfwWindowShouldClose(glfwWindow))
    {
        deltaTime = glfwGetTime() - begin;
        begin = glfwGetTime();

        calcFPS();

        processInput();

        // Set background
        glm::vec3 bgColor = SceneManager::Get().GetSceneBgColor();
        GLCall(glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // ImGui
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui::NewFrame();

        // float old_font_size = ImGui::GetFont()->Scale;
        // ImGui::GetFont()->Scale *= 1.5;
        // ImGui::PushFont(ImGui::GetFont());
        ImGui::Begin("Settings");
        static float lightSizeScale = 0.2f;
        static float waveSpeed = 1.0f;
        static float renderingTime = 0.0f;
        
        ImGui::SliderFloat("Light Size", &lightSizeScale, 0.0f, 1.0f);
        ImGui::SliderFloat("Wave Speed", &waveSpeed, 0.0f, 10.0f);
        ImGui::Text("Rendering time: %fms (%f%%)", renderingTime * 1000, renderingTime/deltaTime*100);
        ImGui::Text("Total time: %fms", deltaTime * 1000);

        ImGui::End();
        // ImGui::GetFont()->Scale = old_font_size;
        // ImGui::TreePop();

        // Update camera
        camera.SetDirection(glm::normalize(callbackData.cameraDirection));
        viewMatrix = camera.GetViewMatrix();

        // put in callbackData
        projectionMatrix = glm::perspective(
            glm::radians(45.0f),
            (float)callbackData.windowWidth/(float)callbackData.windowHeight,
            0.1f,
            100.0f
        );

        // Global uniforms
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo[0]));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix)));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix)));
        float u_time = begin*waveSpeed;
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(GLfloat), &u_time));

        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo[1]));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(camera.cameraPos)));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(glm::vec3), glm::value_ptr(camera.direction)));

        renderingTime = glfwGetTime();
        SceneManager::Get().Update();
        renderingTime = glfwGetTime() - renderingTime;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                
        GLCall(glfwSwapBuffers(glfwWindow));
        GLCall(glfwPollEvents());
    }
}


Window::~Window()
{
    ImGui::Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete these while OpenGL is still in context
    SceneManager::Get().ClearAllScenes();
    Texture::DeleteAllTextures();

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
