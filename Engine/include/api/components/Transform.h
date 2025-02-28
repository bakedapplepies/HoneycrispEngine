#pragma once

#include "api/pch/hncrsp_pch.h"

HNCRSP_NAMESPACE_START

struct Transform
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 eulerAngles = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    bool lookAtCamera = false;
};

void translate(Transform* transform, const glm::vec3& vec);
void scale(Transform* transform, float multiplier);
void rotate(Transform* transform, const glm::vec3& rXYZ);

HNCRSP_NAMESPACE_END