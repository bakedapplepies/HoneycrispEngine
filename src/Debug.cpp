#include <iostream>
#include "Debug.h"

static bool err = false;
void createBreak() {
    err = true;
}

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, unsigned int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OPENGL ERROR] (" << error << ") At: " << function 
            << " Ln: " << line << ", File: " << file << std::endl;
        return false;
    }
    return true;
}