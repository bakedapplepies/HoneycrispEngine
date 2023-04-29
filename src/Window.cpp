#include "pch/pch.h"

#include "Debug.h"
#include "constants.h"
#include "Cube.h"
#include "Light.h"
#include "Window.h"
#include "Callbacks.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"


Window::Window()
{
    /* Initialize GLFW */
    if(!glfwInit())
    {
        std::cout << "GLFW Initialization failed." << '\n';
        // return -1;
    };
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create and assign OpenGL window context */
    glfwSetErrorCallback(error_callback);
    glfwWindow = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        "LearnOpenGL",
        NULL,
        NULL
    );
    if (glfwWindow == NULL)
    {
        std::cout << "GLFW Window Initialization failed." << '\n';
        glfwTerminate();
        // return -1;
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);  // vsync

    /* Callbacks */
    glfwSetWindowUserPointer(glfwWindow, &callbackData);

    glfwSetCursorPosCallback(glfwWindow, mouse_callback);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwSetKeyCallback(glfwWindow, key_callback);

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD Initialization failed." << '\n';
        // return -1;
    }
    std::cout << glGetString(GL_VERSION) << '\n';

    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));


    /* Textures */
    // Image data
    stbi_set_flip_vertically_on_load(true);


    // View Matrix
    viewMatrix = camera.GetViewMatrix();

    // Projection Matrix
    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,
        0.1f,
        100.0f
    );

    for (unsigned int i = 0; i < 2; i++)
    {
        objects.push_back(new Cube(glm::vec3(0.0f, 2*(float)i, 0.0f)));
    }
    objects.push_back(new Light(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    ));

    // ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui::StyleColorsDark();
    // ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_Init();
}


void Window::Loop()
{
    /* Main loop */
    float begin = glfwGetTime();

    while(!glfwWindowShouldClose(glfwWindow))
    {
        deltaTime = glfwGetTime() - begin;
        begin = glfwGetTime();

        calcFPS();

        processInput();

        // Set background
        // GLCall(glClearColor(0.53, 0.81f, 0.92f, 1.0f));
        GLCall(glClearColor(0.09f, 0.09f, 0.09f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // ImGui
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui::NewFrame();

        // Update camera
        camera.SetDirection(glm::normalize(callbackData.direction));
        viewMatrix = camera.GetViewMatrix();

        ImGui::Begin("Demo");
        
        static float lightSizeScale = 0.2f;
        
        ImGui::SliderFloat("Light Size", &lightSizeScale, 0.0f, 1.0f);

        ImGui::End();
        
        
        glm::vec3& color = static_cast<Light*>(objects.back())->GetColor();
        color.r *= cosf(4*begin)/4 + 0.75f;
        color.g *= -sinf(4*begin)/4 + 0.75f;
        color.b *= -sinf(4*begin)/4 + 0.75f;

        // currentShader.Use();
        for (Object*& object : objects)
        {
            object->GetShader().Use();
            modelMatrix = object->GetModelMatrix();
            object->GetShader().setMatrix4Uniform("view", viewMatrix);
            object->GetShader().setMatrix4Uniform("projection", projectionMatrix);


            if (typeid(*object) == typeid(Cube))
            {
                object->GetShader().setMatrix4Uniform("model", modelMatrix);
                object->GetShader().setVector3Uniform("lightColor", color);
                object->GetShader().setVector3Uniform("lightPos", objects.back()->GetPosition());
                object->GetShader().setVector3Uniform("viewPos", camera.cameraPos);

                object->GetShader().setVector3Uniform("material.ambient", glm::vec3(1.0f) * color);
                object->GetShader().setVector3Uniform("material.diffuse", glm::vec3(1.0f) * color);
                object->GetShader().setVector3Uniform("material.specular", glm::vec3(0.5f) * color);
                object->GetShader().setFloatUniform("material.shininess", 32.0f);

                object->GetShader().setVector3Uniform("light.ambient", glm::vec3(0.2f));
                object->GetShader().setVector3Uniform("light.diffuse", glm::vec3(0.5f));
                object->GetShader().setVector3Uniform("light.specular", glm::vec3(1.0f));
                
                // for Phong shading
                object->GetShader().setMatrix3Uniform("normalMatrix", glm::mat4(glm::transpose(glm::inverse(modelMatrix))));
            }
            else if (typeid(*object) == typeid(Light))
            {
                modelMatrix = glm::scale(modelMatrix, glm::vec3(lightSizeScale));
                object->GetShader().setMatrix4Uniform("model", modelMatrix);
                object->GetPosition().x = 1.5f * cosf(begin*1.5f);
                object->GetPosition().z = 1.5f * sinf(begin*1.5f);

                object->GetShader().setVector3Uniform("uColor", color);
            }

            object->Draw();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                
        GLCall(glfwSwapBuffers(glfwWindow));
        GLCall(glfwPollEvents());
    }
}


Window::~Window()
{
    // ImGui::Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (Object*& object : objects)
    {
        delete object;
    }
    std::cout << "Deleted all objects" << '\n';

    glfwTerminate();
}


void Window::calcFPS()
{
    totalTime += deltaTime;
    frames++;
    if (totalTime >= 1.0f)
    {   
        std::string s = "LearnOpenGL - FPS: " + std::to_string(frames);
        glfwSetWindowTitle(glfwWindow, s.c_str());
        frames = 0;
        totalTime = 0.0f;
    }
}

void Window::processInput()
{
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ChangePos(glm::normalize(glm::vec3(camera.cameraDirection.x, 0.0f, camera.cameraDirection.z)) * camera.speed * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ChangePos(-glm::normalize(glm::vec3(camera.cameraDirection.x, 0.0f, camera.cameraDirection.z)) * camera.speed * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.cameraDirection, camera.cameraUp));
        camera.ChangePos(-glm::vec3(direction.x, 0.0f, direction.z) * camera.speed * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.cameraDirection, camera.cameraUp));
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
