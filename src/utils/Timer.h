#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

template <typename T>
class Timer
{
static_assert(std::chrono::__is_duration<T>() && 
    (  std::is_same_v<T, std::chrono::nanoseconds>
    || std::is_same_v<T, std::chrono::microseconds>
    || std::is_same_v<T, std::chrono::milliseconds>
    || std::is_same_v<T, std::chrono::seconds>
    ), "Type of duration not supported.");
private:
    std::chrono::time_point<std::chrono::steady_clock> m_begin;
    std::string_view m_timerName;

public:
    Timer(std::string_view timer_name = "Timer") :
        m_timerName(timer_name)
    {
        m_begin = std::chrono::steady_clock::now();
    }

    ~Timer()
    {
        auto duration = std::chrono::steady_clock::now() - m_begin;
        HNCRSP_LOG_INFO(fmt::format("Timer \"{}\": {}{}",
            m_timerName,
            std::chrono::duration_cast<T>(duration).count(),
            _GetPostfix()
        ));
    }

    // Return current elapsed time since object initialization
    inline int64_t Query()
    {
        return std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - m_begin).count();
    }

private:
    inline const char* _GetPostfix()
    {
        if constexpr(std::is_same_v<T, std::chrono::nanoseconds>)
        {
            return "ns";
        }
        else if constexpr(std::is_same_v<T, std::chrono::microseconds>)
        {
            return "mcs";
        }
        else if constexpr(std::is_same_v<T, std::chrono::milliseconds>)
        {
            return "ms";
        }
        else if constexpr(std::is_same_v<T, std::chrono::seconds>)
        {
            return "s";
        }
        else
        {
            HNCRSP_TERMINATE("Type of duration not supported.");
        }
    }
};

HNCRSP_NAMESPACE_END