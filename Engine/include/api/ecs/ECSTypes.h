#pragma once

#include "api/pch/hncrsp_pch.h"

HNCRSP_NAMESPACE_START

namespace ECS
{
    using EntityUID = std::uint32_t;
    const EntityUID MAX_ENTITIES = 2500;
    const size_t MAX_COMPONENTS = 32;
    using ComponentBitset = std::bitset<MAX_COMPONENTS>;
}

HNCRSP_NAMESPACE_END