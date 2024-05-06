#pragma once

#include "utils.h"


#ifdef HNCRSP_DEBUG
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))
    #define HNCRSP_CHECK_RENDER_CONTEXT() CheckRenderContext(__FUNCTION__, __FILE__, __LINE__)
#else
    #define GLCall(x) x;
    #define HNCRSP_CHECK_RENDER_CONTEXT()
#endif

HNCRSP_NAMESPACE_START

void GLClearError();
bool GLLogCall(const char* function, const char* file, unsigned int line);
void CheckRenderContext(const char* function, const char* file, unsigned int line);

HNCRSP_NAMESPACE_END