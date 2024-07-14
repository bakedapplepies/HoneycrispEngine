#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

template <typename T, typename... Args>
class Command
{
private:
    std::function<T(Args...)> m_function;

public:
    Command(std::function<T(Args...)> func)
    {
        m_function = func;
    }

    T execute(Args&&... args)
    {
        return m_function(std::forward<Args>(args)...);
    }
};

HNCRSP_NAMESPACE_END