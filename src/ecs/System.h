#pragma once

#include "src/pch/pch.h"
#include "ECSTypes.h"


HNCRSP_NAMESPACE_START

class System
{
public:
    std::vector<EntityUID> entityUIDs;

    // Since Renderer has a unique way of storing entities to
    // avoid state changes, might as well make all system do this
    // in case they want to optimize how they store things.
    virtual void AddEntityUID(EntityUID entityUID) = 0;

    virtual ~System() = default;
};

HNCRSP_NAMESPACE_END