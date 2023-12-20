#pragma once

#include "pch/pch.h"
#include "Object.h"


class NonRenderable : public Object
{
public:
    std::vector<glm::vec3> positions;
    
public:
    virtual ~NonRenderable() {}
    virtual inline void addPosition(const glm::vec3& position) { positions.push_back(position); };
};