#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    float fov = 45.0f;
    float nearClippingPlane = 0.1f;
    float farClippingPlane = 100.0f;
    float speed = 6.0f;

public:
    Camera(const glm::vec3& initialPos);
    ~Camera() = default;

    void SetPos(const glm::vec3& newPos);
    void ChangePos(const glm::vec3& posOffset);
    void SetDirection(const glm::vec3& direction);
    
    // Returns a view matrix
    glm::mat4 GetViewMatrix() const;

    // Returns a perspective projection matrix
    glm::mat4 GetProjectionMatrix(uint32_t width, uint32_t height) const;
};

HNCRSP_NAMESPACE_END