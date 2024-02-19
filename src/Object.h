#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

class Object
{
public:
    virtual void OnUpdate() = 0;
    virtual ~Object() = default;
};

HNCRSP_NAMESPACE_END