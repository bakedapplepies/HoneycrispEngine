#pragma once

#include "src/pch/pch.h"
#include "src/managers/SceneManager.h"
#include "src/managers/RenderContext.h"
#include "src/graphics/core/UniformBuffer.h"


HNCRSP_NAMESPACE_START

class Window
{
private:
    GLFWwindow* m_glfwWindow = nullptr;
    CallbackData* m_callbackData = nullptr;

    glm::mat4 projectionMatrix;

    float m_windowWidthScalar;
    float m_windowHeightScalar;

    bool m_continueProgram = true;
    float m_deltaTime = 0.0f;
    float m_totalTime = 0.0f;
    unsigned int m_frames = 0;

    std::vector<std::string> m_pps;

public:
    Window(CallbackData* callbackData);
    ~Window();
    void Loop();


private:
    void CalcFPS();
    void ProcessInput();
    void UpdatePPS();
};

HNCRSP_NAMESPACE_END