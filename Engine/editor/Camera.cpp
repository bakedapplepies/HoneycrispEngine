#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>  // glm::lookAt
#include <glm/ext/matrix_clip_space.hpp>  // glm::perspective

HNCRSP_NAMESPACE_START

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::GetProjectionMatrix(float aspect_ratio) const
{
    return glm::perspective(glm::radians(fov), aspect_ratio, nearPlane, farPlane);
}

void Camera::MoveForward(float distance, float delta_time)
{
    position += distance * glm::normalize(glm::vec3(viewDir.x, 0.0f, viewDir.z)) * delta_time;
}

void Camera::MoveBackward(float distance, float delta_time)
{
    position -= distance * glm::normalize(glm::vec3(viewDir.x, 0.0f, viewDir.z)) * delta_time;
}

void Camera::MoveLeft(float distance, float delta_time)
{
    position += distance * glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), viewDir)) * delta_time;
}

void Camera::MoveRight(float distance, float delta_time)
{
    position -= distance * glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), viewDir)) * delta_time;
}

void Camera::MoveUp(float distance, float delta_time)
{
    position += distance * glm::vec3(0.0f, 1.0f, 0.0f) * delta_time;
}

void Camera::MoveDown(float distance, float delta_time)
{
    position -= distance * glm::vec3(0.0f, 1.0f, 0.0f) * delta_time;
}

HNCRSP_NAMESPACE_END