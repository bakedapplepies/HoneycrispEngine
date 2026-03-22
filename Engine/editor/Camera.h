#pragma once

#include <glm/glm.hpp>

#include "core/Defines.h"

HNCRSP_NAMESPACE_START

class Camera
{
public:
    Camera() = default;
    ~Camera() = default;

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspect_ratio) const;

    void MoveForward(float distance, float delta_time);
    void MoveBackward(float distance, float delta_time);
    void MoveLeft(float distance, float delta_time);
    void MoveRight(float distance, float delta_time);
    void MoveUp(float distance, float delta_time);
    void MoveDown(float distance, float delta_time);

public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
};

HNCRSP_NAMESPACE_END