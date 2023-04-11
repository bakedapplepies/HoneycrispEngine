#pragma once

#include "pch/pch.h"


class Camera
{
public:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 Direction = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f) - cameraPos;

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

    // glm::vec3 cameraUp = glm::cross(cameraFront, cameraRight);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    // glm::mat4 view = glm::lookAt(
    //     cameraPos,
    //     glm::vec3(0.0f, 0.0f, -1.0f),
    //     glm::vec3(0.0f, 1.0f, 0.0f)
    // );
    // glm::mat4 view = glm::lookAt(
    //     cameraPos,
    //     cameraPos + cameraDirection,
    //     cameraUp
    // );

public:
    void SetPos(const glm::vec3& newPos);
    void ChangePos(const glm::vec3& posOffset);
    void SetDirection(const glm::vec3& direction);
    
    glm::mat4 GetViewMatrix();
};