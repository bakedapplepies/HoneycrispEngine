#include "api/core/Debug.h"
#include "api/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

#ifndef HNCRSP_RELEASE

void GLAPIENTRY DebugMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    [[maybe_unused]] GLsizei length,
    const GLchar* message,
    [[maybe_unused]] const void* userParam
) {
    // Ignore certain verbose info messages (particularly ones on Nvidia).
    if (id == 131169 || 
        id == 131185 || // NV: Buffer will use video memory
        id == 131218 || 
        id == 131204 || // Texture cannot be used for texture mapping
        id == 131222 ||
        id == 131154 || // NV: pixel transfer is synchronized with 3D rendering
        id == 0         // gl{Push, Pop}DebugGroup
    ) { return; }

    std::stringstream debugMessageStream;
    debugMessageStream << message << '\n';

    switch (source)
    {
        case GL_DEBUG_SOURCE_API: debugMessageStream << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: debugMessageStream << "Source: Window Manager"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: debugMessageStream << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: debugMessageStream << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: debugMessageStream << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER: debugMessageStream << "Source: Other"; break;
    }

    debugMessageStream << '\n';

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR: debugMessageStream << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: debugMessageStream << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: debugMessageStream << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY: debugMessageStream << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: debugMessageStream << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER: debugMessageStream << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: debugMessageStream << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP: debugMessageStream << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER: debugMessageStream << "Type: Other"; break;
    }

    debugMessageStream << '\n';

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: debugMessageStream << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: debugMessageStream << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW: debugMessageStream << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: debugMessageStream << "Severity: notification"; break;
    }

    HNCRSP_ERROR("OpenGL Message: {} {}", type, debugMessageStream.str());
    return;
}   

void CheckRenderContext(const char* function, const char* file, unsigned int line)
{
    if (!glfwGetCurrentContext())
    {
        std::cout << fmt::format("Render context not available at: {} | {}:{}\n", function, file, line);
        assert(false);
    }
}

#else

void GLAPIENTRY DebugMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    [[maybe_unused]] GLsizei length,
    const GLchar* message,
    [[maybe_unused]] const void* userParam
) {}

void CheckRenderContext(const char* function, const char* file, unsigned int line) {}

#endif

HNCRSP_NAMESPACE_END