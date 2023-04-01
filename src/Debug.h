#pragma once

#include <glad/glad.h>

#define ASSERT(x) if (!(x)) createBreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void createBreak();

void GLClearError();
bool GLLogCall(const char* function, const char* file, unsigned int line);