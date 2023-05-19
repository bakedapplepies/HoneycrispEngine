#pragma once

#include "pch/pch.h"

#include "Cube.h"
#include "Light.h"
#include "constants.h"
#include "Object.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"


struct CallbackData  // maybe make private
{
    bool capFPS = true;
    bool showMouse = false;
    bool firstMouse = true;
    float lastX = WINDOW_WIDTH/2;
    float lastY = WINDOW_HEIGHT/2;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
    glm::vec3 cameraDirection;
};

class Window
{
private:
    GLFWwindow* glfwWindow;
    CallbackData callbackData;
    Camera camera;
    std::unique_ptr<Cube> cube;
    std::unique_ptr<Light> light;

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
    ~Window();

    void Loop();
};