#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>  // glm::lookAt
#include <glm/ext/matrix_clip_space.hpp>  // glm::perspective


glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::GetProjectionMatrix(float aspect_ratio) const
{
    return glm::perspective(glm::radians(fov), aspect_ratio, nearPlane, farPlane);
}