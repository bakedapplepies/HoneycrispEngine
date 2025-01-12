#include "WindowHandler.h"
#include "src/Callbacks.h"

HNCRSP_NAMESPACE_START

void WindowHandler::StartUp()
{
    if (glfwGetCurrentContext())
    {
        HNCRSP_ERROR("GLFW already initialized.");
        return;
    }
        
    if (!glfwInit())
    {
        HNCRSP_TERMINATE("GLFW Initialization failed.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create and assign OpenGL window context ----------
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_callbackData.windowWidth = mode->width * 0.75f;
    m_callbackData.windowHeight = mode->height * 0.75f;
    HNCRSP_INFO("Resolution: {}x{}", m_callbackData.windowWidth, m_callbackData.windowHeight);
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
    glfwSetWindowPos(glfwWindow, 0, 80);
    glfwSwapInterval(0);  // vsync
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Callbacks ----------
    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(glfwWindow, mouse_callback);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwSetKeyCallback(glfwWindow, key_callback);

    glfwSetWindowUserPointer(glfwWindow, &m_callbackData);
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // disable cursor

    // Set Icon ----------
    FileSystem::Path icon("resources/textures/apple.png");
    GLFWimage image[1];
    image[0].pixels = stbi_load(icon.string().c_str(), &image[0].width, &image[0].height, nullptr, 4);
    glfwSetWindowIcon(glfwWindow, 1, image);
    stbi_image_free(image->pixels);

    HNCRSP_INFO("Window Initialization done.");
}

void WindowHandler::ShutDown()
{
    glfwTerminate();
}

HNCRSP_NAMESPACE_END