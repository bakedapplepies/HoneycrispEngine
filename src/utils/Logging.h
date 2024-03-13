#pragma once

#include <iostream>
#include "src/utils/utils.h"


std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec2);
std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec3);

HNCRSP_NAMESPACE_START

static bool __HNCRSP_INTERAL_USE_ONLY_beganLine = true;

template <typename T>
static void __Info(const char* file, size_t line, const T& t)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[INFO ~{}:{}] ", file, line);
    }
    std::cout << t << '\n';
    __HNCRSP_INTERAL_USE_ONLY_beganLine = true;
}

template <typename T, typename... Args>
static void __Info(const char* file, size_t line, const T& t, const Args&... args)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[INFO ~{}:{}] ", file, line);
    }
    __HNCRSP_INTERAL_USE_ONLY_beganLine = false;
    std::cout << t;

    __Info(file, line, args...);
}

template <typename T>
static void __Warn(const char* file, size_t line, const T& t)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[WARN ~{}:{}] ", file, line);
    }
    std::cout << t << '\n';
    __HNCRSP_INTERAL_USE_ONLY_beganLine = true;
}

template <typename T, typename... Args>
static void __Warn(const char* file, size_t line, const T& t, const Args&... args)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[WARN ~{}:{}] ", file, line);
    }
    __HNCRSP_INTERAL_USE_ONLY_beganLine = false;
    std::cout << t;

    __Warn(file, line, args...);
}

template <typename T>
static void __Error(const char* file, size_t line, const T& t)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[ERROR ~{}:{}] ", file, line);
    }
    std::cout << t << '\n';
    __HNCRSP_INTERAL_USE_ONLY_beganLine = true;
}

template <typename T, typename... Args>
static void __Error(const char* file, size_t line, const T& t, const Args&... args)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[ERROR ~{}:{}] ", file, line);
    }
    __HNCRSP_INTERAL_USE_ONLY_beganLine = false;
    std::cout << t;

    __Error(file, line, args...);
}


HNCRSP_NAMESPACE_END

#define HNCRSP_LOG_INFO(...)  Honeycrisp::__Info(__FILE__, __LINE__, __VA_ARGS__)
#define HNCRSP_LOG_WARN(...)  Honeycrisp::__Warn(__FILE__, __LINE__, __VA_ARGS__)
#define HNCRSP_LOG_ERROR(...) Honeycrisp::__Error(__FILE__, __LINE__, __VA_ARGS__)