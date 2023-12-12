#pragma once

#include "../pch/pch.h"


#define TERMINATE(msg) terminate(#msg, __FILE__, __LINE__);

// #define DEBUG
#define ASSERT(x) if (!(x)) createBreak();
// #define DEBUG
// #ifdef DEBUG
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __BASE_FILE__, __LINE__))
// #else
// #define GLCall(x) x;
// #endif

#define GRAY(x) system("Color 08");\
    x;\
    system("Color 07");\


void createBreak();

void GLClearError();
bool GLLogCall(const char* function, const char* file, unsigned int line);
void terminate(const char* msg, const char* file, unsigned int line);

static std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec2)
{
    std::cout << fmt::format("[{}, {}]", vec2.x, vec2.y);
    return stream;
}

static std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec3)
{
    std::cout << fmt::format("[{}, {}, {}]", vec3.x, vec3.y, vec3.z);
    return stream;
}

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
    void Log(const T& t, const Args&... args)
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
    void Warn(const T& t, const Args&... args)
    {
        if (beganLine)
        {
            std::cout << "[WARN ~" << __BASE_FILE__ << "]: ";
            beganLine = false;
        }
        std::cout << t;

        Warn(args...);
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
    void Error(const T& t, const Args&... args)
    {
        if (beganLine)
        {
            std::cout << "[ERROR ~" << __BASE_FILE__ << "]: ";
            beganLine = false;
        }
        std::cout << t;

        Error(args...);
    }
};