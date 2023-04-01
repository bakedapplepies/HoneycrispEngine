#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"


class Window
{
private:
    GLFWwindow* glfwWindow;
    Camera camera;
    VertexArray currentVAO;
    Texture currentTexture;
    Shader currentShader;

    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float deltaTime;
    float totalTime = 0;
    unsigned int frames = 0;

private:
    void calcFPS();
    void processInput();

public:
    Window();

    void Loop();
};