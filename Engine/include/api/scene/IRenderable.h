#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/ecs/ECSTypes.h"
#include "api/graphics/Material.h"

HNCRSP_NAMESPACE_START

class IRenderable
{
public:
    // called in scenes
    virtual void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID, const Material& material) const = 0;
    virtual ~IRenderable() = default;
};

HNCRSP_NAMESPACE_END