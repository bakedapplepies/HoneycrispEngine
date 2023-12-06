#pragma once

#include "pch/pch.h"

#include "core/Shader.h"


class Object
{
protected:
    std::vector<glm::vec3> positions;

public:
    virtual inline void addPosition(const glm::vec3& position) { positions.push_back(position); };
    virtual void OnUpdate() = 0;
};