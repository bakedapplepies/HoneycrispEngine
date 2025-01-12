#pragma once

#include "api/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class DepthPassCamera
{
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction;
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    float fov = 45.0f;
    float nearClippingPlane = 0.1f;
    float farClippingPlane = 50.0f;
    float speed = 6.0f;
    float distOffMainCam = 200.0f;

public:
    DepthPassCamera() = default;
    DepthPassCamera(const glm::vec3& initial_pos);
    ~DepthPassCamera() = default;

    void SetPos(const glm::vec3& new_pos);
    void ChangePos(const glm::vec3& pos_offset);
    void SetDirection(const glm::vec3& direction);
    glm::vec3 GetDirection() const;
    
    // Returns a view matrix
    glm::mat4 GetViewMatrix(const glm::vec3& light_pos, const glm::vec3& target) const;

    // Returns an orthographic projection matrix
    glm::mat4 GetProjectionMatrix() const;

    glm::mat4 GetViewProjectionMatrix(const glm::vec3& target) const;
};

HNCRSP_NAMESPACE_END