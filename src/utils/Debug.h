#pragma once


#ifdef HNCRSP_DEBUG
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __BASE_FILE__, __LINE__))
#else
    #define GLCall(x) x;
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, unsigned int line);