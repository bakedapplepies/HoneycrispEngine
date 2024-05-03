#pragma once

#include "src/pch/pch.h"
#include "src/managers/SceneManager.h"
#include "src/managers/RenderContext.h"
#include "opengl/UniformBuffer.h"
#include "Camera.h"


HNCRSP_NAMESPACE_START

class Window
{
private:
    GLFWwindow* m_glfwWindow = nullptr;
    RenderContext::CallbackData* m_callbackData = nullptr;

    Camera camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f));  // set initial direction in CallbackData

    glm::mat4 projectionMatrix;

    float m_windowWidthScalar;
    float m_windowHeightScalar;

    bool m_continueProgram = true;
    float m_deltaTime = 0.0f;
    float m_totalTime = 0.0f;
    unsigned int m_frames = 0;

    std::vector<std::string> m_pps;

public:
    Window(RenderContext::CallbackData* callbackData);
    ~Window();
    void Loop();


private:
    void calcFPS();
    void processInput();
    void updatePPS();
};

HNCRSP_NAMESPACE_END