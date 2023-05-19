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

Debug::Debug()
{
    file.open(filepath);
}

Debug::~Debug()
{
    file.close();
}

Debug& Debug::Get()
{
    return self;
}

Debug Debug::self;
// std::ostream Debug::file(self.filepath);

template <typename T>
void Debug::Log(const T& t)
{
    file << t << '\n';
    std::cout << t << '\n';
}

template <typename T, typename... Args>
void Debug::Log(const T& t, Args... args)
{
    file << t;
    std::cout << t;

    Log(args...);
}