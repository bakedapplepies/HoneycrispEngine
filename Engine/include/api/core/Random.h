#pragma once

#include "api/pch/hncrsp_pch.h"

HNCRSP_NAMESPACE_START

template <typename T = float>
class Random
{
HNCRSP_STATIC_ASSERT(std::is_arithmetic_v<T>, "Type T of Random class is not an arithmetic type.");
private:
    static std::random_device m_rd;
    static std::mt19937 m_mt;

public:
    static T dist(T begin, T end)
    {
        if constexpr(std::is_floating_point_v<T>)
        {
            std::uniform_real_distribution<T> dist(begin, end);
            return dist(m_mt);
        }
        else if constexpr(std::is_integral_v<T>)
        {
            std::uniform_int_distribution<T> dist(begin, end);
            return dist(m_mt);
        }
    }
};

template <typename T>
inline std::random_device Random<T>::m_rd;
template <typename T>
inline std::mt19937 Random<T>::m_mt(m_rd());

HNCRSP_NAMESPACE_END