#include <iostream>
#include "Debug.h"

bool err = false;
void createBreak() {
    err = true;
    // After temporary breakpoint is created, create
    // another breakpoint at point of error
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