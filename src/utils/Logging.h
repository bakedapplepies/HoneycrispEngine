#pragma once

#include <iostream>
#include "src/utils/FileSystem.h"
#include "src/utils/utils.h"


std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec2);
std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec3);
std::ostream& operator<<(std::ostream& stream, const Honeycrisp::FileSystem::Path& path);

HNCRSP_NAMESPACE_START

static bool __HNCRSP_INTERAL_USE_ONLY_beganLine = true;

template <typename T>
static void __Info(const FileSystem::Path& file, size_t line, const T& t)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[INFO ~{}:{}] ", file.relative_string(), line);
    }
    std::cout << t << '\n';
    __HNCRSP_INTERAL_USE_ONLY_beganLine = true;
}

template <typename T, typename... Args>
static void __Info(const FileSystem::Path& file, size_t line, const T& t, const Args&... args)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[INFO ~{}:{}] ", file.relative_string(), line);
    }
    __HNCRSP_INTERAL_USE_ONLY_beganLine = false;
    std::cout << t;

    __Info(file, line, args...);
}

template <typename T>
static void __Warn(const FileSystem::Path& file, size_t line, const T& t)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[WARN ~{}:{}] ", file.relative_string(), line);
    }
    std::cout << t << '\n';
    __HNCRSP_INTERAL_USE_ONLY_beganLine = true;
}

template <typename T, typename... Args>
static void __Warn(const FileSystem::Path& file, size_t line, const T& t, const Args&... args)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[WARN ~{}:{}] ", file.relative_string(), line);
    }
    __HNCRSP_INTERAL_USE_ONLY_beganLine = false;
    std::cout << t;

    __Warn(file, line, args...);
}

template <typename T>
static void __Error(const FileSystem::Path& file, size_t line, const T& t)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[ERROR ~{}:{}] ", file.relative_string(), line);
    }
    std::cout << t << '\n';
    __HNCRSP_INTERAL_USE_ONLY_beganLine = true;
}

template <typename T, typename... Args>
static void __Error(const FileSystem::Path& file, size_t line, const T& t, const Args&... args)
{
    if (__HNCRSP_INTERAL_USE_ONLY_beganLine)
    {
        std::cout << fmt::format("[ERROR ~{}:{}] ", file.relative_string(), line);
    }
    __HNCRSP_INTERAL_USE_ONLY_beganLine = false;
    std::cout << t;

    __Error(file, line, args...);
}


HNCRSP_NAMESPACE_END

#define HNCRSP_LOG_INFO(...)  Honeycrisp::__Info(FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__)
#define HNCRSP_LOG_WARN(...)  Honeycrisp::__Warn(FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__)
#define HNCRSP_LOG_ERROR(...) Honeycrisp::__Error(FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__)