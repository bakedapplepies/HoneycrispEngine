#pragma once

#include "src/pch/pch.h"
#include "ECSTypes.h"


HNCRSP_NAMESPACE_START

class System
{
public:
    std::vector<EntityUID> entityUIDs;
};

HNCRSP_NAMESPACE_END