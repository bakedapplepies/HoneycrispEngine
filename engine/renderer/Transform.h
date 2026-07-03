#pragma once

#include <glm/glm.hpp>

#include "core/Defines.h"

HNCRSP_NAMESPACE_START

class Transform
{
public:
    Transform() = default;
    ~Transform() = default;

    glm::mat4 GetModelMatrix() const;

public:
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
};

HNCRSP_NAMESPACE_END