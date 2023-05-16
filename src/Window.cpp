#include "pch/pch.h"
#include <typeinfo>

#include "Debug.h"
#include "Window.h"
#include "Callbacks.h"


Texture grassTextureMap;
Window::Window()
{
    /* Initialize GLFW */
    if(!glfwInit())
    {
        std::cout << "GLFW Initialization failed." << '\n';
        // return -1;
    }
    else
    {
        std::cout << "GLFW Initialization success." << '\n';
    }
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

    std::cout << "Window config done." << '\n';

    /* Textures */
    // Image data
    stbi_set_flip_vertically_on_load(true);
    grassTextureMap.LoadTexture(
        "resources/textures/grass_textures.png"
    );
    grassTextureMap.Bind(GL_TEXTURE0);
    std::cout << "Texture loaded." << '\n';


    // View Matrix
    viewMatrix = camera.GetViewMatrix();

    // Projection Matrix
    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,
        0.1f,
        100.0f
    );
    std::cout << "Matrices initialized." << '\n';

    // for (unsigned int i = 0; i < 2; i++)
    // {
    //     std::cout << "i: " << i << '\n';
    //     objects.emplace_back(new Cube(glm::vec3(0.0f, 2*(float)i, 0.0f)));
    // }
    // objects.emplace_back(new Light(
    //     glm::vec3(1.0f, 1.0f, 1.0f),
    //     glm::vec3(1.0f, 1.0f, 1.0f)
    // ));

    cube = Cube(glm::vec3(0.0f, 0.0f, 0.0f));
    light = Light(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    std::cout << 'l' << '\n';

    // ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init();

    std::cout << "Window Initialization done." << '\n';
}


void Window::Loop()
{
    /* Main loop */
    float begin = glfwGetTime();

    std::cout << "Entering loop." << '\n';

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
        camera.SetDirection(glm::normalize(callbackData.cameraDirection));
        viewMatrix = camera.GetViewMatrix();

        ImGui::Begin("Demo");
        
        static float lightSizeScale = 0.2f;
        
        ImGui::SliderFloat("Light Size", &lightSizeScale, 0.0f, 1.0f);

        ImGui::End();
        
        
        glm::vec3& color = light.GetColor();
        color.r *= cosf(4*begin)/4 + 0.75f;
        color.g *= -sinf(4*begin)/4 + 0.75f;
        color.b *= -sinf(4*begin)/4 + 0.75f;

        // currentShader.Use();
        cube.GetShader().Use();
        modelMatrix = cube.GetModelMatrix();
        cube.GetShader().setMatrix4Uniform("view", viewMatrix);
        cube.GetShader().setMatrix4Uniform("projection", projectionMatrix);

        cube.GetShader().setMatrix4Uniform("model", modelMatrix);
        cube.GetShader().setVector3Uniform("lightColor", color);
        cube.GetShader().setVector3Uniform("lightPos", light.GetPosition());
        cube.GetShader().setVector3Uniform("viewPos", camera.cameraPos);

        cube.GetShader().setVector3Uniform("material.ambient", glm::vec3(1.0f) * color);
        cube.GetShader().setVector3Uniform("material.diffuse", glm::vec3(1.0f) * color);
        cube.GetShader().setVector3Uniform("material.specular", glm::vec3(0.5f) * color);
        cube.GetShader().setFloatUniform("material.shininess", 32.0f);

        cube.GetShader().setVector3Uniform("light.ambient", glm::vec3(0.2f));
        cube.GetShader().setVector3Uniform("light.diffuse", glm::vec3(0.5f));
        cube.GetShader().setVector3Uniform("light.specular", glm::vec3(1.0f));
        
        // for Phong shading
        cube.GetShader().setMatrix3Uniform("normalMatrix", glm::mat4(glm::transpose(glm::inverse(modelMatrix))));


        light.GetShader().Use();
        modelMatrix = light.GetModelMatrix();
        light.GetShader().setMatrix4Uniform("view", viewMatrix);
        light.GetShader().setMatrix4Uniform("projection", projectionMatrix);

        modelMatrix = glm::scale(modelMatrix, glm::vec3(lightSizeScale));
        light.GetShader().setMatrix4Uniform("model", modelMatrix);
        light.GetPosition().x = 1.5f * cosf(begin*1.5f);
        light.GetPosition().z = 1.5f * sinf(begin*1.5f);

        light.GetShader().setVector3Uniform("uColor", color);

        light.Draw();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                
        GLCall(glfwSwapBuffers(glfwWindow));
        GLCall(glfwPollEvents());
    }
}


Window::~Window()
{
    std::cout << "Deallocating resources." << '\n';
    grassTextureMap.Delete();

    // ImGui::Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // for (Object*& object : objects)
    // {
    //     std::cout << typeid(*object).name() << '\n';
    //     delete object;
    // }

    std::cout << "Deleted all objects" << '\n';

    glfwTerminate();
    std::cin.get();
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
