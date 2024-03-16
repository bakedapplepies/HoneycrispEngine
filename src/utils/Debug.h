#pragma once

#include "utils.h"


#ifdef HNCRSP_DEBUG
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __BASE_FILE__, __LINE__))
#else
    #define GLCall(x) x;
#endif

HNCRSP_NAMESPACE_START

void GLClearError();
bool GLLogCall(const char* function, const char* file, unsigned int line);

HNCRSP_NAMESPACE_END