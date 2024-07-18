#include "RenderContext.h"
#include "src/Callbacks.h"


HNCRSP_NAMESPACE_START

HNCRSP_NODISCARD CallbackData* RenderContext::StartUp_GetWindow()
{
    // Create and assign OpenGL window context ----------
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_callbackData.windowWidth = mode->width * 0.75f;
    m_callbackData.windowHeight = mode->height * 0.75f;
    HNCRSP_LOG_INFO(fmt::format("Resolution: {}x{}", m_callbackData.windowWidth, m_callbackData.windowHeight));
    GLFWwindow* glfwWindow = glfwCreateWindow(
        m_callbackData.windowWidth,
        m_callbackData.windowHeight,
        "Honeycrisp",
        nullptr,
        nullptr
    );

    if (glfwWindow == nullptr)
    {
        HNCRSP_TERMINATE("GLFW Window Initialization failed.");
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);  // vsync
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Callbacks ----------
    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(glfwWindow, mouse_callback);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwSetKeyCallback(glfwWindow, key_callback);

    glfwSetWindowUserPointer(glfwWindow, &m_callbackData);
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // disable cursor

    /* Initialize GLAD -> Only call OpenGL functions after this */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        HNCRSP_TERMINATE("GLAD Initialization failed.");
    }
    HNCRSP_LOG_INFO("OpenGL (Core) ", glGetString(GL_VERSION));
    
    // Set Icon ----------
    FileSystem::Path icon("resources/textures/apple.png");
    GLFWimage image[1];
    image[0].pixels = stbi_load(icon.string().c_str(), &image[0].width, &image[0].height, nullptr, 4);
    glfwSetWindowIcon(glfwWindow, 1, image);
    stbi_image_free(image->pixels);

    /* Depth, Stencil, Blending, Gamma correction */
    // depth test
    glEnable(GL_DEPTH_TEST);

    // cull faces
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // stencil test
    glEnable(GL_STENCIL_TEST);

    // blending
    glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  TODO
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    // multisample anti-aliasing
    glEnable(GL_MULTISAMPLE);

    // gamma correction
    // glEnable(GL_FRAMEBUFFER_SRGB);

    glViewport(0, 0, static_cast<unsigned int>(m_callbackData.windowWidth * 0.8f), m_callbackData.windowHeight);

    return &m_callbackData;
}

HNCRSP_NAMESPACE_END