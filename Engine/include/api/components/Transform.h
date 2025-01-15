#pragma once

#include "api/pch/hncrsp_pch.h"


struct Transform
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 eulerAngles = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    bool lookAtCamera = false;

    Transform() = default;
    explicit Transform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale, bool look_at_camera = false)
        : position(position), eulerAngles(eulerAngles), scale(scale), lookAtCamera(look_at_camera)
    {}
    Transform(const Transform& other);
    Transform& operator=(const Transform& other);
    Transform(Transform&& other) noexcept;
    Transform& operator=(Transform&& other) noexcept;
    ~Transform() = default;

    // void translate(const glm::vec3& vec);
    // void scale(const float& multiplier);
    // void rotate(const float& rX, const float& rY, const float& rZ);
};