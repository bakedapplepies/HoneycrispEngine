#pragma once

#include "pch/pch.h"

#include "Shader.h"


class Object
{
protected:
    std::vector<glm::vec3> positions;

public:
    virtual void AddPosition(const glm::vec3& position) = 0;
    virtual std::vector<glm::vec3>& GetPositions() = 0;
};