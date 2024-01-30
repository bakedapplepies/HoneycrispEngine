#include "RenderContext.h"


HNCRSP_NAMESPACE_START

namespace RenderContext
{
    namespace
    {
        CallbackData callbackData;
    }

    [[nodiscard]] WindowPtr_and_CallbackDataPtr StartUp_GetWindow()
    {
        /* Create and assign OpenGL window context */
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        callbackData.windowWidth = mode->width * 0.75f;
        callbackData.windowHeight = mode->height * 0.75f;
        HNCRSP_LOG_INFO(callbackData.windowWidth);
        HNCRSP_LOG_INFO(callbackData.windowHeight);
        GLFWwindow* glfwWindow = glfwCreateWindow(
            callbackData.windowWidth,
            callbackData.windowHeight,
            "LearnOpenGL",
            nullptr,
            nullptr
        );

        if (glfwWindow == nullptr)
        {
            HNCRSP_TERMINATE("GLFW Window Initialization failed.");
        }
        glfwMakeContextCurrent(glfwWindow);
        glfwSwapInterval(1);  // vsync

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

        /* Depth, Stencil, Blending */
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

        GLCall(glViewport(500, 0, callbackData.windowWidth-500, callbackData.windowHeight));

        return { glfwWindow, &callbackData };
    }
}

HNCRSP_NAMESPACE_END