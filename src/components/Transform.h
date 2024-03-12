#pragma once

#include "src/pch/pch.h"


struct Transform
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 eulerAngles = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    Transform() = default;
    explicit Transform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
        : position(position), eulerAngles(eulerAngles), scale(scale)
    {}
    Transform(const Transform& other);
    Transform& operator=(const Transform& other);
    Transform(Transform&& other) noexcept;
    Transform& operator=(Transform&& other) noexcept;
    ~Transform() = default;
};