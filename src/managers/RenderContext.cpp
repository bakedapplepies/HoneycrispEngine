#include "RenderContext.h"
#include "src/utils/Debug.h"
#include "src/managers/WindowHandler.h"


HNCRSP_NAMESPACE_START

void RenderContext::StartUp()
{
    CallbackData* callbackData = GetCallbackData();

    /* Initialize GLAD -> Only call OpenGL functions after this */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        HNCRSP_TERMINATE("GLAD Initialization failed.");
    }
    HNCRSP_INFO("OpenGL (Core) ", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    // Set DebugCallback ----------
    glDebugMessageCallback(DebugMessageCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    /* Depth, Stencil, Blending, Gamma correction */
    // cull faces
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // stencil test
    glEnable(GL_STENCIL_TEST);

    // blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // TODO
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    // multisample anti-aliasing
    glEnable(GL_MULTISAMPLE);

    // gamma correction
    // glEnable(GL_FRAMEBUFFER_SRGB);

    glViewport(0, 0, static_cast<unsigned int>(callbackData->windowWidth * 0.8f), callbackData->windowHeight);
}

HNCRSP_NAMESPACE_END