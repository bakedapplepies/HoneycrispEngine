#pragma once


#ifndef HNCRSP_RELEASE
    #define HNCRSP_CHECK_RENDER_CONTEXT() CheckRenderContext(__FUNCTION__, __FILE__, __LINE__)
#else
    #define HNCRSP_CHECK_RENDER_CONTEXT()
#endif

HNCRSP_NAMESPACE_START

void GLAPIENTRY DebugMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    [[maybe_unused]] GLsizei length,
    const GLchar* message,
    [[maybe_unused]] const void* userParam
);
void CheckRenderContext(const char* function, const char* file, unsigned int line);

HNCRSP_NAMESPACE_END