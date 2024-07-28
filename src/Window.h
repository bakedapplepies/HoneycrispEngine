#pragma once

#include "src/pch/hncrsp_pch.h"
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
    void _CalcFPS();
    void _ProcessInput();
    void _UpdatePPS();
};

HNCRSP_NAMESPACE_END