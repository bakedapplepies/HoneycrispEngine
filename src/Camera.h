#pragma once

#include "pch/pch.h"


class Camera
{
public:
    glm::vec3 cameraPos;
    glm::vec3 direction;
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    float speed = 6.0f;

public:
    Camera(const glm::vec3& initialPos);
    ~Camera() = default;

    void SetPos(const glm::vec3& newPos);
    void ChangePos(const glm::vec3& posOffset);
    void SetDirection(const glm::vec3& direction);
    
    glm::mat4 GetViewMatrix() const;
};