#pragma once

#include <glad/glad.h>
#include "api/core/Defines.h"

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
    HNCRSP_MAYBE_UNUSED GLsizei length,
    const GLchar* message,
    HNCRSP_MAYBE_UNUSED const void* userParam
);

void CheckRenderContext(const char* function, const char* file, unsigned int line);

HNCRSP_NAMESPACE_END