#pragma once

#include "src/pch/pch.h"
#include "src/ecs/ECSManager.h"
#include "src/opengl/VertexArray.h"
#include "src/Object.h"


HNCRSP_NAMESPACE_START

class Renderable : public Object
{
public:
    // called in scenes
    virtual void virt_AddMeshDataToRenderer(EntityUID entityUID) = 0;
};

HNCRSP_NAMESPACE_END