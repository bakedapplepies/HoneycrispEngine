#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Material.h"
#include "src/ecs/ECSTypes.h"
#include "src/Object.h"


HNCRSP_NAMESPACE_START

class Renderable // : public Object
{
public:
    // called in scenes
    virtual void virt_AddDrawDataToRenderer(EntityUID entityUID) = 0;
    virtual ~Renderable() = default;
};

HNCRSP_NAMESPACE_END