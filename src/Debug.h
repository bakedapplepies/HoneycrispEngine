#pragma once

#include "pch/pch.h"

#include <glad/glad.h>

#define ASSERT(x) if (!(x)) createBreak();
#define GLCall(x) x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void createBreak();

void GLClearError();
bool GLLogCall(const char* function, const char* file, unsigned int line);

class Debug
{
private:
    Debug();
    Debug(const Debug&) = delete;
    Debug(Debug&&) noexcept = delete;
    ~Debug();

    static Debug self;

    std::string filepath = "log/debug.log";
    std::ofstream file;

public:
    static Debug& Get();

    template <typename T>
    void Log(const T& t);

    template <typename T, typename... Args>
    void Log(const T& t, Args... args);
};