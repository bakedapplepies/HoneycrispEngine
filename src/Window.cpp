#include "pch/pch.h"
#include <typeinfo>

#include "Debug.h"
#include "Window.h"
#include "Callbacks.h"


Window::Window()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    
    /* Initialize GLFW */
    if(!glfwInit())
    {
        Debug::Error("GLFW Initialization failed.");
        // return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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

    /* Callbacks */
    glfwSetWindowUserPointer(glfwWindow, &callbackData);

    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(glfwWindow, mouse_callback);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwSetKeyCallback(glfwWindow, key_callback);


    /* Initialize GLAD -> Only call OpenGL functions after this */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Debug::Error("GLAD Initialization failed.");
        glfwTerminate();
    }
    Debug::Log("Version: ", glGetString(GL_VERSION));


    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glFrontFace(GL_CW));
    GLCall(glCullFace(GL_BACK));
    // GLCall(glEnable(GL_BLEND));
    GLCall(glViewport(0, 0, callbackData.windowWidth, callbackData.windowHeight));

    Debug::Log("Window config done.");

    /* Textures */
    // Image data
    stbi_set_flip_vertically_on_load(true);
    Texture::LoadTextures();
    Debug::Log("Texture loaded.");

    // View Matrix
    viewMatrix = camera.GetViewMatrix();

    // Projection Matrix
    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)callbackData.windowWidth/(float)callbackData.windowHeight,
        0.1f,
        100.0f
    );

    cube = std::make_unique<Cube>();
    for (int i = -5; i < 5; i++)
    {
        for (int j = -5; j < 5; j++)
        {
            cube->AddPosition(glm::vec3(j, i, 0.0f));
        }
    }

    light = std::make_unique<Light>(
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    light->AddPosition(glm::vec3(1.0f, 1.0f, 3.0f));

    TextureCoords& grassUV = Textures::mainTextureMap.GetTextureCoords(0, 0);
    mesh = std::make_unique<Mesh>(
        std::vector<float>({
            -8.0f,  0.0f, -8.0f,
             8.0f,  0.0f, -8.0f,
             8.0f,  0.0f,  8.0f,
            -8.0f,  0.0f,  8.0f,
        }),
        std::vector<float>({
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
        }),
        std::vector<float>({
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
        }),
        std::vector<float>({
            grassUV.tl.x, grassUV.tl.y,
            grassUV.tr.x, grassUV.tr.y,
            grassUV.br.x, grassUV.br.y,
            grassUV.bl.x, grassUV.bl.y
        }),
        std::vector<unsigned int>({
            0, 1, 2,
            0, 2, 3
        })
    );
    mesh->AddPosition(glm::vec3(0.0f, -8.0f, 0.0f));
    mesh->GetShader() = Shader(
        "../resources/shaders/vertex.vert",
        "../resources/shaders/fragment.frag"
    );

    // ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init();

    Debug::Log("Window Initialization done.");
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
        GLCall(glClearColor(0.09f, 0.09f, 0.09f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // ImGui
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui::NewFrame();

        // Update camera
        camera.SetDirection(glm::normalize(callbackData.cameraDirection));
        viewMatrix = camera.GetViewMatrix();

        ImGui::Begin("Settings");
        
        static float lightSizeScale = 0.2f;
        
        ImGui::SliderFloat("Light Size", &lightSizeScale, 0.0f, 1.0f);

        ImGui::End();

        // put in callbackData
        projectionMatrix = glm::perspective(
            glm::radians(45.0f),
            (float)callbackData.windowWidth/(float)callbackData.windowHeight,
            0.1f,
            100.0f
        );

        // texture bindings
        Textures::mainTextureMap.Bind();
        Textures::mainTextureSpecularMap.Bind();
    
        glm::vec3& lightColor =
        light->GetColor();
        // std::string light = fmt::format("({}, {}, {})", lightColor.r, lightColor.g, lightColor.b);
        // Debug::Log(light.c_str());
        lightColor.r = cosf(4*begin)/4 + 0.75f;
        lightColor.g = -sinf(4*begin)/4 + 0.75f;
        lightColor.b = -sinf(4*begin)/4 + 0.75f;

        cube->GetShader().Use();

        cube->GetShader().setMatrix4Uniform("u_view", viewMatrix);
        cube->GetShader().setMatrix4Uniform("u_projection", projectionMatrix);

        cube->GetShader().setVector3Uniform("u_viewPos", camera.cameraPos);

        cube->GetShader().setIntUniform("u_material.diffuse", Textures::mainTextureMap.getTextureUnit());
        cube->GetShader().setIntUniform("u_material.specular", Textures::mainTextureSpecularMap.getTextureUnit());
        cube->GetShader().setFloatUniform("u_material.shininess", 32.0f);

        cube->GetShader().setVector3Uniform("u_light.position", camera.cameraPos);
        cube->GetShader().setVector3Uniform("u_light.direction", camera.cameraDirection);
        cube->GetShader().setFloatUniform("u_light.cutOff", glm::cos(glm::radians(15.0f)));
        cube->GetShader().setFloatUniform("u_light.outerCutOff", glm::cos(glm::radians(25.0f)));

        cube->GetShader().setVector3Uniform("u_light.ambient", 0.1f * lightColor);
        cube->GetShader().setVector3Uniform("u_light.diffuse", 0.5f * lightColor);
        cube->GetShader().setVector3Uniform("u_light.specular", 1.0f * lightColor);

        cube->GetShader().setFloatUniform("u_light.constant", 1.0f);
        cube->GetShader().setFloatUniform("u_light.linear", 0.045f);
        cube->GetShader().setFloatUniform("u_light.quadratic", 0.0075f);
        
        // for Phong shading
        cube->GetShader().setMatrix3Uniform("u_normalMatrix", glm::mat4(glm::transpose(glm::inverse(modelMatrix))));

        for (const glm::vec3& i_position : cube->GetPositions())
        {
            modelMatrix = cube->GetModelMatrix(i_position);
            cube->GetShader().setMatrix4Uniform("u_model", modelMatrix);

            cube->Draw();
        }

        // cube and mesh have the same shader so maybe stop including shader inside objects
        // or somehow group them (static shader members inside the shader class?)

        for (const glm::vec3& i_position : mesh->GetPositions())
        {
            modelMatrix = mesh->GetModelMatrix(i_position);
            mesh->GetShader().setMatrix4Uniform("u_model", modelMatrix);
            
            mesh->Draw();
        }


        // light->GetShader().Use();
        // light->GetShader().setMatrix4Uniform("view", viewMatrix);
        // light->GetShader().setMatrix4Uniform("projection", projectionMatrix);
        // for (glm::vec3& i_position : light->GetPositions())
        // {
        //     modelMatrix = light->GetModelMatrix(i_position);

        //     modelMatrix = glm::scale(modelMatrix, glm::vec3(lightSizeScale));
        //     light->GetShader().setMatrix4Uniform("model", modelMatrix);
        //     i_position.x = 1.5f * cosf(begin*1.5f);
        //     i_position.z = 1.5f * sinf(begin*1.5f);

        //     light->GetShader().setVector3Uniform("uColor", color);

        //     light->Draw();
        // }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                
        GLCall(glfwSwapBuffers(glfwWindow));
        GLCall(glfwPollEvents());
    }
}


Window::~Window()
{
    Debug::Log("Deallocating resources.");

    // ImGui::Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    Texture::DeleteAllTextures();
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
