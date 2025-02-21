#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/ecs/ECSTypes.h"

HNCRSP_NAMESPACE_START

struct SceneObject
{
    std::vector<ECS::EntityUID> children;
};

HNCRSP_NAMESPACE_END