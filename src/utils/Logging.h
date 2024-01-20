#pragma once

#include <iostream>
#include "src/utils/utils.h"


std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec2);
std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec3);

HNCRSP_NAMESPACE_START

namespace Log
{
    namespace
    {
        bool beganLine = true;
    }

    template <typename T>
    void Info(const char* file, size_t line, const T& t)
    {
        if (beganLine)
        {
            std::cout << fmt::format("[INFO ~{}:{}] ", file, line);
        }
        std::cout << t << '\n';
        beganLine = true;
    }
    
    template <typename T, typename... Args>
    void Info(const char* file, size_t line, const T& t, const Args&... args)
    {
        if (beganLine)
        {
            std::cout << fmt::format("[INFO ~{}:{}] ", file, line);
            beganLine = false;
        }
        std::cout << t;

        Info(file, line, args...);
    }

    template <typename T>
    void Warn(const char* file, size_t line, const T& t)
    {
        if (beganLine)
        {
            std::cout << fmt::format("[WARN ~{}:{}] ", file, line);
        }
        std::cout << t << '\n';
        beganLine = true;
    }

    template <typename T, typename... Args>
    void Warn(const char* file, size_t line, const T& t, const Args&... args)
    {
        if (beganLine)
        {
            std::cout << fmt::format("[WARN ~{}:{}] ", file, line);
            beganLine = false;
        }
        std::cout << t;

        Warn(file, line, args...);
    }

    template <typename T>
    void Error(const char* file, size_t line, const T& t)
    {
        if (beganLine)
        {
            std::cout << fmt::format("[ERROR ~{}:{}] ", file, line);
        }
        std::cout << t << '\n';
        beganLine = true;
    }

    template <typename T, typename... Args>
    void Error(const char* file, size_t line, const T& t, const Args&... args)
    {
        if (beganLine)
        {
            std::cout << fmt::format("[ERROR ~{}:{}] ", file, line);
            beganLine = false;
        }
        std::cout << t;

        Error(file, line, args...);
    }
}

HNCRSP_NAMESPACE_END

#define HNCRSP_LOG_INFO(...)  Honeycrisp::Log::Info(__FILE__, __LINE__, __VA_ARGS__)
#define HNCRSP_LOG_WARN(...)  Honeycrisp::Log::Warn(__FILE__, __LINE__, __VA_ARGS__)
#define HNCRSP_LOG_ERROR(...) Honeycrisp::Log::Error(__FILE__, __LINE__, __VA_ARGS__)