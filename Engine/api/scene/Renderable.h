#pragma once

#include "api/pch/hncrsp_pch.h"
#include "src/ecs/ECSTypes.h"


HNCRSP_NAMESPACE_START

class Renderable
{
public:
    // called in scenes
    virtual void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID) const = 0;
    virtual ~Renderable() = default;
};

HNCRSP_NAMESPACE_END