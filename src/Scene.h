#pragma once

#include "pch/pch.h"
#include "Object.h"

enum EObjectRenderType
{
    STATIC,
    DYNAMIC
};

class Scene
{
protected:
    std::vector<Object> objectRefs;
    // glm::vec3 bgColor;
    
protected:
    Object CreateObject(Object&&, EObjectRenderType render_type);
};