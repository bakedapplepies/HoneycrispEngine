#pragma once

#include "pch/pch.h"

#include "Mesh.h"
#include "Cube.h"
#include "Light.h"
#include "Texture.h"
#include "Camera.h"


struct CallbackData
{
    bool capFPS = true;
    bool showMouse = false;
    bool firstMouse = true;
    int windowHeight;
    int windowWidth;
    float lastX;
    float lastY;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
    glm::vec3 cameraDirection = glm::vec3(0, 0, -1.0f);  // initial camera direction or it would look at origin by default
};

class Application;
class Window
{
private:
    GLFWwindow* glfwWindow;
    CallbackData callbackData;
    Camera camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f));  // set initial direction in CallbackData
    std::unique_ptr<Cube> cube;
    Light light;
    Mesh mesh;
    Shader mainShader;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    bool continueProgram = true;
    float deltaTime;
    float totalTime = 0;
    unsigned int frames = 0;

private:
    void calcFPS();
    void processInput();

public:
    Window();
    ~Window();

    void Loop(std::shared_ptr<Application> app);
};