#include "src/pch/pch.h"

#include "Camera.h"


HNCRSP_NAMESPACE_START

Camera::Camera(const glm::vec3& initialPos)
{
    position = initialPos;
}

void Camera::SetPos(const glm::vec3& newPos)
{
    position = newPos;
}

void Camera::ChangePos(const glm::vec3& posOffset)
{
    position += posOffset;
}

void Camera::SetDirection(const glm::vec3& newDirection)
{
    direction = newDirection;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(
        position,
        position + direction,
        cameraUp
    );
}

HNCRSP_NAMESPACE_END