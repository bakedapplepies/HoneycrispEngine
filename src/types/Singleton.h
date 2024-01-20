#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

class Singleton
{
protected:
    Singleton() = default;
    ~Singleton() = default;
    
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

HNCRSP_NAMESPACE_END