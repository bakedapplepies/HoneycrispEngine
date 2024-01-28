#pragma once

#include "src/pch/pch.h"

#include "src/core/RenderContext.h"
#include "renderer/UniformBuffer.h"
#include "Camera.h"


HNCRSP_NAMESPACE_START

class Window
{
private:
    GLFWwindow* m_glfwWindow;
    RenderContext::CallbackData* m_callbackData;
    Camera camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f));  // set initial direction in CallbackData

    glm::mat4 projectionMatrix;

    bool continueProgram = true;
    float deltaTime;
    float totalTime = 0.0f;
    unsigned int frames = 0;

public:
    void StartUp(GLFWwindow* glfwWindow, RenderContext::CallbackData* callbackData);
    void Loop();
    Window() = default;
    ~Window() = default;


private:
    void calcFPS();
    void processInput();
};

HNCRSP_NAMESPACE_END