#include "RenderContext.h"
#include "src/Callbacks.h"


HNCRSP_NAMESPACE_START

namespace RenderContext
{
    namespace
    {
        CallbackData callbackData;
    }

    [[nodiscard]] CallbackData* StartUp_GetWindow()
    {
        /* Create and assign OpenGL window context */
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        callbackData.windowWidth = mode->width * 0.75f;
        callbackData.windowHeight = mode->height * 0.75f;
        HNCRSP_LOG_INFO(fmt::format("Resolution: {}x{}", callbackData.windowWidth, callbackData.windowHeight));
        GLFWwindow* glfwWindow = glfwCreateWindow(
            callbackData.windowWidth,
            callbackData.windowHeight,
            "Honeycrisp",
            nullptr,
            nullptr
        );

        if (glfwWindow == nullptr)
        {
            HNCRSP_TERMINATE("GLFW Window Initialization failed.");
        }
        glfwMakeContextCurrent(glfwWindow);
        glfwSwapInterval(0);  // vsync

        /* Callbacks */
        glfwSetErrorCallback(error_callback);
        glfwSetCursorPosCallback(glfwWindow, mouse_callback);
        glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
        glfwSetKeyCallback(glfwWindow, key_callback);

        glfwSetWindowUserPointer(glfwWindow, &callbackData);
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // disable cursor

        /* Initialize GLAD -> Only call OpenGL functions after this */
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            HNCRSP_TERMINATE("GLAD Initialization failed.");
        }
        HNCRSP_LOG_INFO("OpenGL (Core) ", glGetString(GL_VERSION));

        /* Depth, Stencil, Blending, Gamma correction */
        // depth test
            GLCall(glEnable(GL_DEPTH_TEST));
        // cull faces
            GLCall(glEnable(GL_CULL_FACE));
            GLCall(glFrontFace(GL_CW));
            GLCall(glCullFace(GL_BACK));
        // stencil test
            GLCall(glEnable(GL_STENCIL_TEST));
        // blending
            // GLCall(glEnable(GL_BLEND));
        // gamma correction
            GLCall(glEnable(GL_FRAMEBUFFER_SRGB));

        int viewportWidth = (1.0f - callbackData.settingsWidthPercentage) * callbackData.windowWidth;
        GLCall(glViewport(callbackData.windowWidth - viewportWidth, 0, viewportWidth, callbackData.windowHeight));

        return &callbackData;
    }
}

HNCRSP_NAMESPACE_END