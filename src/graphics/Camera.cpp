#include "src/pch/hncrsp_pch.h"

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

glm::mat4 Camera::GetProjectionMatrix(uint32_t width, uint32_t height) const
{
    return glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(width)/static_cast<float>(height),
        nearClippingPlane,
        farClippingPlane
    );
}

HNCRSP_NAMESPACE_END