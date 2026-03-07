#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera() = default;
    ~Camera() = default;

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspect_ratio) const;

public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
};