#pragma once

#include "pch/pch.h"


#define ASSERT(x) if (!(x)) createBreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __BASE_FILE__, __LINE__))


void createBreak();

void GLClearError();
bool GLLogCall(const char* function, const char* file, unsigned int line);

namespace Debug
{
    namespace
    {
        bool beganLine = true;
    }

    template <typename T>
    void Log(const T& t)
    {
        if (beganLine)
        {
            std::cout << "[LOG ~" << __BASE_FILE__ << "]: ";
        }
        std::cout << t << '\n';
        beganLine = true;
    }

    template <typename T, typename... Args>
    void Log(const T& t, Args... args)
    {
        if (beganLine)
        {
            std::cout << "[LOG ~" << __BASE_FILE__ << "]: ";
            beganLine = false;
        }
        std::cout << t;

        Log(args...);
    }

    template <typename T>
    void Warn(const T& t)
    {
        if (beganLine)
        {
            std::cout << "[WARN ~" << __BASE_FILE__ << "]: ";
        }
        std::cout << t << '\n';
        beganLine = true;
    }

    template <typename T, typename... Args>
    void Warn(const T& t, Args... args)
    {
        if (beganLine)
        {
            std::cout << "[WARN ~" << __BASE_FILE__ << "]: ";
            beganLine = false;
        }
        std::cout << t;

        Log(args...);
    }

    template <typename T>
    void Error(const T& t)
    {
        if (beganLine)
        {
            std::cout << "[ERROR ~" << __BASE_FILE__ << "]: ";
        }
        std::cout << t << '\n';
        beganLine = true;
    }

    template <typename T, typename... Args>
    void Error(const T& t, Args... args)
    {
        if (beganLine)
        {
            std::cout << "[ERROR ~" << __BASE_FILE__ << "]: ";
            beganLine = false;
        }
        std::cout << t;

        Log(args...);
    }
};