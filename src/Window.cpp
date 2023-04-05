#include "pch/pch.h"

#include "Debug.h"
#include "constants.h"
#include "Cube.h"
#include "Window.h"
#include "Callbacks.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
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
    // glfwSwapInterval(1);  // vsync

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


    /* Vertex Buffer */
    float vertices[] = {
//       x      y      z      r      g      b      tx    ty
        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // front top left - 0
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // front top right - 1
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // front bottom right - 2
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // front bottom left - 3
        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4
         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // back top right - 5
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // back bottom left - 7
    };

    /* Element Buffer Object */
    unsigned int indicies[] = {
        0, 1, 2,  // front
        2, 3, 0,
        1, 5, 6, // right
        6, 2, 1,
        5, 4, 7,  // back
        7, 6, 5,
        4, 0, 3,  // left
        3, 7, 4,
        4, 5, 1,  // top
        1, 0, 4,
        3, 2, 6,  // bottom
        6, 7, 3,
    };

    /* Textures */
    // Image data
    stbi_set_flip_vertically_on_load(true);


    /* Shader Programs */
    currentShader.CreateShader(
        "resources/shaders/vertex.vert",
        "resources/shaders/fragment.frag"
    );


    /* Enable texture units */
    currentShader.Use();
    GLCall(glUniform1i(glGetUniformLocation(currentShader.getID(), "uTexture0"), 0));
    
    /* Camera stuff */
    // Model matrix

    // View Matrix
    viewMatrix = camera.getViewMatrix();

    // Projection Matrix
    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,
        0.1f,
        100.0f
    );
    currentShader.setMatrixfvUniform("projection", projectionMatrix);

    currentVAO.Unbind();

    for (unsigned int i = 0; i < 10; i++)
    {
        objects.push_back(new Cube(0.0f, (float)i, 0.0f));
    }
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
        GLCall(glClearColor(0.53, 0.81f, 0.92f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // Update camera
        camera.SetDirection(glm::normalize(callbackData.direction));
        viewMatrix = camera.getViewMatrix();
        currentShader.setMatrixfvUniform("view", viewMatrix);


        currentShader.Use();
        for (Object*& object : objects)
        {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, object->GetPosition());
            currentShader.setMatrixfvUniform("model", modelMatrix);
            object->Draw();
        }
                
        GLCall(glfwSwapBuffers(glfwWindow));
        GLCall(glfwPollEvents());
    }
}


Window::~Window()
{
    for (Object* object : objects)
    {
        delete object;
    }

    currentVAO.Delete();
    currentTexture.Delete();  // can produce overhead
    currentShader.Delete();

    glfwTerminate();
}


void Window::calcFPS()
{
    totalTime += deltaTime;
    frames++;
    if (totalTime >= 1.0f)
    {   
        std::stringstream ss;
        ss << "LearnOpenGL - FPS: " << frames;
        glfwSetWindowTitle(glfwWindow, ss.str().c_str());
        frames = 0;
        totalTime = 0.0f;
    }
}

void Window::processInput()
{
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ChangePos(glm::normalize(glm::vec3(camera.cameraDirection.x, 0.0f, camera.cameraDirection.z)) * 3.0f * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ChangePos(-glm::normalize(glm::vec3(camera.cameraDirection.x, 0.0f, camera.cameraDirection.z)) * 3.0f * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.cameraDirection, camera.cameraUp));
        camera.ChangePos(-glm::vec3(direction.x, 0.0f, direction.z) * 3.0f * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(camera.cameraDirection, camera.cameraUp));
        camera.ChangePos(glm::vec3(direction.x, 0.0f, direction.z) * 3.0f * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.ChangePos(glm::vec3(0.0f, 1.0f, 0.0f) * 3.0f * deltaTime);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ChangePos(glm::vec3(0.0f, -1.0f, 0.0f) * 3.0f * deltaTime);
    }
}
