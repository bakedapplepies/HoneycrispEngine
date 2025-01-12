#include "api/graphics/Camera.h"

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

glm::vec3 Camera::GetLeftVec() const
{
    return glm::normalize(glm::cross(cameraUp, glm::vec3(direction.x, 0.0f, direction.z)));
}

glm::vec3 Camera::GetRightVec() const
{
    return glm::normalize(glm::cross(glm::vec3(direction.x, 0.0f, direction.z), cameraUp));
}

glm::mat4 Camera::GetProjectionMatrix(uint32_t width, uint32_t height) const
{
    return glm::perspective(
        glm::radians(fov),
        static_cast<float>(width)/static_cast<float>(height),
        nearClippingPlane,
        farClippingPlane
    );
}

glm::mat4 Camera::GetOrthoMatrix(float left, float right, float bottom, float top) const
{
    return glm::ortho(
        left, right, bottom, top, nearClippingPlane, farClippingPlane
    );
}

Camera::Camera(const Camera& other)
{
    position = other.position;
    direction = other.direction;
    cameraUp = other.cameraUp;
    fov = other.fov;
    nearClippingPlane = other.nearClippingPlane;
    farClippingPlane = other.farClippingPlane;
    speed = other.speed;
}

Camera& Camera::operator=(const Camera& other)
{
    position = other.position;
    direction = other.direction;
    cameraUp = other.cameraUp;
    fov = other.fov;
    nearClippingPlane = other.nearClippingPlane;
    farClippingPlane = other.farClippingPlane;
    speed = other.speed;

    return *this;
}

Camera::Camera(Camera&& other) noexcept
{
    position = other.position;
    direction = other.direction;
    cameraUp = other.cameraUp;
    fov = other.fov;
    nearClippingPlane = other.nearClippingPlane;
    farClippingPlane = other.farClippingPlane;
    speed = other.speed;

    other.position = glm::vec3(0.0f);
    other.direction = glm::vec3(0.0f, 0.0f, 1.0f);
    other.cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
    other.fov = 45.0f;
    other.nearClippingPlane = 0.1f;
    other.farClippingPlane = 100.0f;
    other.speed = 6.0f;
}

Camera& Camera::operator=(Camera&& other) noexcept
{
    position = other.position;
    direction = other.direction;
    cameraUp = other.cameraUp;
    fov = other.fov;
    nearClippingPlane = other.nearClippingPlane;
    farClippingPlane = other.farClippingPlane;
    speed = other.speed;

    other.position = glm::vec3(0.0f);
    other.direction = glm::vec3(0.0f, 0.0f, 1.0f);
    other.cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
    other.fov = 45.0f;
    other.nearClippingPlane = 0.1f;
    other.farClippingPlane = 100.0f;
    other.speed = 6.0f;
    return *this;
}

HNCRSP_NAMESPACE_END