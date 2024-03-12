#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

using EntityUID = std::uint32_t;
const EntityUID MAX_ENTITIES = 5000;
const size_t MAX_COMPONENTS = 32;
using ComponentBitset = std::bitset<MAX_COMPONENTS>;

HNCRSP_NAMESPACE_END