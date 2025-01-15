#pragma once

#include "api/pch/hncrsp_pch.h"

HNCRSP_NAMESPACE_START

class Camera
{
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    float fov = 45.0f;
    float nearClippingPlane = 0.1f;
    float farClippingPlane = 100.0f;
    float speed = 6.0f;

public:
    Camera() = default;
    Camera(const glm::vec3& initialPos);
    Camera(const Camera& other);
    Camera& operator=(const Camera& other);
    Camera(Camera&& other) noexcept;
    Camera& operator=(Camera&& other) noexcept;
    ~Camera() = default;

    void SetPos(const glm::vec3& newPos);
    void ChangePos(const glm::vec3& posOffset);
    void SetDirection(const glm::vec3& direction);
    
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(uint32_t width, uint32_t height) const;
    glm::mat4 GetOrthoMatrix(float left, float right, float bottom, float top) const;

    glm::vec3 GetLeftVec() const;
    glm::vec3 GetRightVec() const;
};

HNCRSP_NAMESPACE_END