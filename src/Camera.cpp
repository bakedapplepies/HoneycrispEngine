#include "src/pch/pch.h"

#include "Camera.h"


HNCRSP_NAMESPACE_START

Camera::Camera(const glm::vec3& initialPos)
{
    cameraPos = initialPos;
    cameraPos - glm::vec3(0.0f, 0.0f, 0.0f);
}

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
    direction = newDirection;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(
        cameraPos,
        cameraPos + direction,
        cameraUp
    );
}

HNCRSP_NAMESPACE_END