#pragma once

#include "src/pch/pch.h"
#include "src/core/Material.h"
#include "src/ecs/ECSTypes.h"
#include "src/Object.h"


HNCRSP_NAMESPACE_START

class Renderable // : public Object
{
public:
    // called in scenes
    virtual void virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material = nullptr) = 0;
    virtual ~Renderable() = default;
};

HNCRSP_NAMESPACE_END