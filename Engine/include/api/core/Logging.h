#pragma once

#include "api/pch/hncrsp_pch.h"

extern std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec2);
extern std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec3);
extern std::ostream& operator<<(std::ostream& stream, const glm::mat3& mat3);
extern std::ostream& operator<<(std::ostream& stream, const glm::mat4& mat4);
extern std::ostream& operator<<(std::ostream& stream, const Honeycrisp::FileSystem::Path& path);

template <>
class fmt::formatter<glm::vec2>
{
public:
    constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }

    template <typename Context>
    constexpr auto format (const glm::vec2& vec2, Context& ctx) const
    {
        return format_to(ctx.out(), "[{}, {}]", vec2.x, vec2.y);
    }
};

template <>
class fmt::formatter<glm::vec3>
{
public:
    constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }

    template <typename Context>
    constexpr auto format (const glm::vec3& vec3, Context& ctx) const
    {
        return format_to(ctx.out(), "[{}, {}, {}]", vec3.x, vec3.y, vec3.z);
    }
};

template <>
class fmt::formatter<glm::mat3>
{
public:
    constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }

    template <typename Context>
    constexpr auto format (const glm::mat3& mat3, Context& ctx) const
    {
        return format_to(ctx.out(), "{}", glm::to_string(mat3));
    }
};

template <>
class fmt::formatter<glm::mat4>
{
public:
    constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }

    template <typename Context>
    constexpr auto format (const glm::mat4& mat4, Context& ctx) const
    {
        return format_to(ctx.out(), "{}", glm::to_string(mat4));
    }
};

template <>
class fmt::formatter<Honeycrisp::FileSystem::Path>
{
public:
    constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }

    template <typename Context>
    constexpr auto format (const Honeycrisp::FileSystem::Path& path, Context& ctx) const
    {
        return format_to(ctx.out(), "{}", path.string());
    }
};

template <>
class fmt::formatter<std::thread::id>
{
public:
    constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }

    template <typename Context>
    constexpr auto format (const std::thread::id& thread_id, Context& ctx) const
    {
        return format_to(ctx.out(), "{}", std::hash<std::thread::id>{}(thread_id));
    }
};

HNCRSP_NAMESPACE_START

enum class LogLevel: uint32_t
{
    LVL_FATAL = 0,
    LVL_ERROR,
    LVL_WARN,
    LVL_INFO,
    LVL_DEBUG
};

static const char* LOG_LEVELS[] = {
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"
};

template <typename... Args>
static void __Log(
    LogLevel log_level,
    const FileSystem::Path& file,
    size_t line,
    fmt::format_string<Args...> format_str,
    Args&&... args
) {
    fmt::print("[{} ~{}:{}] {}\n",
        LOG_LEVELS[static_cast<uint32_t>(log_level)],
        file.engine_relative_string(),
        line,
        fmt::format(format_str, std::forward<Args>(args)...)
    );
}

HNCRSP_NAMESPACE_END

// Adjust Logging settings here
#define HNCRSP_LOG_FATAL_ENABLED 1
#define HNCRSP_LOG_ERROR_ENABLED 1
#define HNCRSP_LOG_WARN_ENABLED  1
#define HNCRSP_LOG_INFO_ENABLED  1

#if HNCRSP_RELEASE
#   define HNCRSP_LOG_DEBUG_ENABLED 0
#else
#   define HNCRSP_LOG_DEBUG_ENABLED 1
#endif

#if HNCRSP_LOG_FATAL_ENABLED == 1
#   define HNCRSP_FATAL(...) \
        Honeycrisp::__Log(LogLevel::LVL_FATAL, FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__)
#else
#   define HNCRSP_FATAL(...)
#endif

#if HNCRSP_LOG_ERROR_ENABLED == 1
#   define HNCRSP_ERROR(...) \
        Honeycrisp::__Log(LogLevel::LVL_ERROR, FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__)
#else
#   define HNCRSP_ERROR(...)
#endif

#if HNCRSP_LOG_WARN_ENABLED == 1
#   define HNCRSP_WARN(...) \
        Honeycrisp::__Log(LogLevel::LVL_WARN, FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__)
#else
#   define HNCRSP_WARN(...)
#endif

#if HNCRSP_LOG_INFO_ENABLED == 1
#   define HNCRSP_INFO(...) \
        Honeycrisp::__Log(LogLevel::LVL_INFO, FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__)
#else
#   define HNCRSP_INFO(...)
#endif

#if HNCRSP_LOG_DEBUG_ENABLED == 1
#   define HNCRSP_DEBUG(...) \
        Honeycrisp::__Log(LogLevel::LVL_DEBUG, FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__)
#else
#   define HNCRSP_DEBUG(...)
#endif