#pragma once

#include "pch/pch.h"

#include "constants.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"


struct CallbackData  // maybe make private
{
    bool firstMouse = true;
    float lastX = WINDOW_WIDTH/2;
    float lastY = WINDOW_HEIGHT/2;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
    glm::vec3 direction;
};

class Window
{
private:
    GLFWwindow* glfwWindow;
    CallbackData callbackData;
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

    // static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    // static void error_callback(int error, const char *msg);
    // static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    Window();

    void Loop();
};