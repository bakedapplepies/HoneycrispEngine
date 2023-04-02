#include "pch/pch.h"

#include "Debug.h"
#include "constants.h"
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


    /* Vertex Buffer */
    float vertices[] = {
//       x      y      z      r      g      b      tx    ty
        // -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // front top left - 0
        //  0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // front top right - 1
        //  0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // front bottom right - 2
        // -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // front bottom left - 3
        // -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4
        //  0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // back top right - 5
        //  0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
        // -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // back bottom left - 7


        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // front top left - 0
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // front top right - 1
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // front bottom right - 2
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // front bottom right - 2
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // front bottom left - 3
        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // front top left - 0

         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  0.0f, 1.0f,  // front top right - 1
         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // back top right - 5
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  0.0f, 0.0f,  // front bottom right - 2
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  0.0f, 1.0f,  // front top right - 1

         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  0.0f, 1.0f,  // back top right - 5
        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  1.0f, 1.0f,  // back top left - 4
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  1.0f, 0.0f,  // back bottom left - 7
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  1.0f, 0.0f,  // back bottom left - 7
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  0.0f, 0.0f,  // back bottom right - 6
         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  0.0f, 1.0f,  // back top right - 5

        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4
        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  1.0f, 1.0f,  // front top left - 0
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  1.0f, 0.0f,  // front bottom left - 3
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  1.0f, 0.0f,  // front bottom left - 3
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // back bottom left - 7
        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4

        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4
         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // back top right - 5
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 0.0f,  // front top right - 1
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 0.0f,  // front top right - 1
        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 0.0f,  // front top left - 0
        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4

        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 1.0f,  // front bottom left - 3
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 1.0f,  // front bottom right - 2
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // back bottom left - 7
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 1.0f,  // front bottom left - 3
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

    /* Vertex Array Object */
    currentVAO.CreateVAO(vertices, sizeof(vertices), indicies, sizeof(indicies), GL_STATIC_DRAW);
    currentVAO.getEBO()->Unbind();
    currentVAO.Bind();

    /* Textures */
    // Texture Rendering parameters

    // Image data
    stbi_set_flip_vertically_on_load(true);
    currentTexture.CreateTexture("resources/textures/grass_block_side.png");

    
    /* Vertex Attributes */
    // Position XYZ
    GLCall(glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    ));
    GLCall(glEnableVertexAttribArray(0));

    // Color RGB
    GLCall(glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    ));
    GLCall(glEnableVertexAttribArray(1));

    // Texture Coordinates XY (2D)
    GLCall(glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    ));
    GLCall(glEnableVertexAttribArray(2));


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
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // View Matrix

    viewMatrix = camera.view;

    // Projection Matrix
    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,
        0.1f,
        100.0f
    );
    currentShader.setMatrixfvUniform("projection", projectionMatrix);

    currentVAO.Unbind();
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

        GLCall(glClearColor(0.18f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // defaultShader.setFloatUniform("time", begin);

        currentTexture.Bind();

        currentShader.Use();

        modelMatrix = glm::rotate(modelMatrix, sinf(begin)/10, glm::vec3(0.0f, 1.0f, 1.0f));

        camera.cameraDirection = glm::normalize(callbackData.direction);
        viewMatrix = glm::lookAt(
            camera.cameraPos,
            camera.cameraPos + camera.cameraDirection,
            camera.cameraUp
        );

        currentShader.setMatrixfvUniform("model", modelMatrix);
        currentShader.setMatrixfvUniform("view", viewMatrix);

        currentVAO.Bind();
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        
        GLCall(glfwSwapBuffers(glfwWindow));  // swap double buffers
        GLCall(glfwPollEvents());
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
        std::cout << "FPS: " << frames << '\n';
        frames = 0;
        totalTime = 0.0f;
    }
}

void Window::processInput()
{
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.cameraPos += 3.0f * camera.cameraDirection * deltaTime;
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.cameraPos -= 3.0f * camera.cameraDirection * deltaTime;
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.cameraPos -= glm::normalize(glm::cross(camera.cameraDirection, camera.cameraUp)) * 3.0f * deltaTime;
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.cameraPos += glm::normalize(glm::cross(camera.cameraDirection, camera.cameraUp)) * 3.0f * deltaTime;
    }
}
