#pragma once

#include "src/pch/pch.h"


class Object
{
public:
    virtual void OnUpdate() = 0;
    virtual ~Object() = default;
};