#include "pch/pch.h"

#include "Camera.h"


void Camera::SetPos(const glm::vec3& newPos)
{
    cameraPos = newPos;
}

void Camera::ChangePos(const glm::vec3& posOffset)
{
    cameraPos += posOffset;
}

void Camera::SetDirection(const glm::vec3& newDirection)
{
    cameraDirection = newDirection;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(
        cameraPos,
        cameraPos + cameraDirection,
        cameraUp
    );
}
