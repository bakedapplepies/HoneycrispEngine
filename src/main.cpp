#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Callbacks.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>



const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 800;

static float totalTime = 0.0f;
static unsigned int frames = 0;
void calcFPS(float dt)
{
    totalTime += dt;
    frames++;
    if (totalTime >= 1.0f)
    {
        std::cout << "FPS: " << frames << '\n';
        frames = 0;
        totalTime = 0.0f;
    }
}

int main(void)
{
    /* Initialize GLFW */
    if(!glfwInit())
    {
        std::cout << "GLFW Initialization failed." << '\n';
        return -1;
    };
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create and assign OpenGL window context */
    glfwSetErrorCallback(error_callback);
    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        "LearnOpenGL",
        NULL,
        NULL
    );
    if (window == NULL)
    {
        std::cout << "GLFW Window Initialization failed." << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // vsync

    /* Callbacks */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD Initialization failed." << '\n';
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << '\n';

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    /* Vertex Array Object */
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* Vertex Buffer */
    float vertices[] = {
//       x      y      z      r      g      b      tx    ty
        -0.5f,  0.5f,  0.0f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.0f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.0f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,
    };

    GLuint VBO;  // kinda like index of VBO, could also be an array; e.g. VBO[5];
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // VBOs have a buffer type of GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Element Buffer Object */
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0,
    };

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);


    /* Textures */
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(
        "D:/JohnB/Programming/Python/PathFinder/resources/icons/32_pathfinder_icon.png", &width, &height, &nrChannels, 4);
    
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Texture failed to load." << '\n';
    }

    stbi_image_free(data);

    
    /* Vertex Attributes */
    // Position XYZ
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // Color RGB
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Texture Coordinates XY (2D)
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    /* Shader Programs */
    Shader defaultShader(
        "resources/shaders/vertex.vert",   // Path to vertex shader
        "resources/shaders/fragment.frag"  // Path to fragment shader
    );

    defaultShader.useShader();
    glUniform1i(glGetUniformLocation(defaultShader.getID(), "texture0"), 0);


    /* Main loop */
    float deltaTime;
    float begin = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        deltaTime = glfwGetTime() - begin;
        begin = glfwGetTime();

        calcFPS(deltaTime);

        glClearColor(0.18f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // defaultShader.setFloatUniform("time", begin);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        defaultShader.useShader();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        // glBindVertexArray(0);

        // std::cout << glGetError() << '\n';
        
        glfwSwapBuffers(window);  // swap double buffers
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    defaultShader.deleteShader();

    glfwTerminate();
    return 0;
}