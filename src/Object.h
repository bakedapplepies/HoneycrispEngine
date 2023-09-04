#pragma once

#include "pch/pch.h"

#include "Shader.h"


class Object
{
protected:
    std::vector<glm::vec3> positions;

public:
    virtual inline void AddPosition(const glm::vec3& position) { positions.push_back(position); };
    virtual inline std::vector<glm::vec3>& GetPositions() final { return positions; };
};