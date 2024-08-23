#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

template <typename T = float>
class RandomClass
{
static_assert(std::is_floating_point_v<T>, "Type T of Random class is not an floating-point type.");
private:
    static std::random_device m_rd;
    static std::mt19937 m_mt;

public:
    static T dist(T begin, T end)
    {
        std::uniform_real_distribution<T> dist(begin, end);
        return dist(m_mt);
    }
};

template <typename T>
inline std::random_device RandomClass<T>::m_rd;
template <typename T>
inline std::mt19937 RandomClass<T>::m_mt(m_rd());

HNCRSP_NAMESPACE_END