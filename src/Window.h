#pragma once

#include "src/core/SceneManager.h"
#include "src/core/RenderContext.h"
#include "opengl/UniformBuffer.h"
#include "Camera.h"


HNCRSP_NAMESPACE_START

class Window
{
private:
    GLFWwindow* m_glfwWindow;
    RenderContext::CallbackData* m_callbackData;

    Camera camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f));  // set initial direction in CallbackData

    glm::mat4 projectionMatrix;

    float m_windowWidthScalar;
    float m_windowHeightScalar;

    bool m_continueProgram = true;
    float m_deltaTime;
    float m_totalTime = 0.0f;
    unsigned int m_frames = 0;

public:
    void StartUp(RenderContext::CallbackData* callbackData);
    void Loop();
    Window() = default;
    ~Window() = default;


private:
    void calcFPS();
    void processInput();
};

HNCRSP_NAMESPACE_END