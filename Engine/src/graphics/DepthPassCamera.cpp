#include "api/graphics/DepthPassCamera.h"

HNCRSP_NAMESPACE_START

DepthPassCamera::DepthPassCamera(const glm::vec3& initial_pos)
{
    position = initial_pos;
}

void DepthPassCamera::SetPos(const glm::vec3& new_pos)
{
    position = new_pos;
}

void DepthPassCamera::ChangePos(const glm::vec3& pos_offset)
{
    position += pos_offset;
}

void DepthPassCamera::SetDirection(const glm::vec3& new_direction)
{
    direction = glm::normalize(new_direction);
}

glm::vec3 DepthPassCamera::GetDirection() const
{
    return direction;
}

glm::mat4 DepthPassCamera::GetViewMatrix(const glm::vec3& light_pos, const glm::vec3& target) const
{
    return glm::lookAt(
        light_pos,
        light_pos + direction,
        cameraUp
    );
}

glm::mat4 DepthPassCamera::GetProjectionMatrix(float resolution) const
{
    return glm::ortho(
        -10.0f * resolution,  // left
         10.0f * resolution,  // right
        -10.0f * resolution,  // bottom
         10.0f * resolution,  // top,
        nearClippingPlane,
        farClippingPlane
    );
}

glm::mat4 DepthPassCamera::GetViewProjectionMatrix(const glm::vec3& target, float resolution) const
{
    return GetProjectionMatrix(resolution) * GetViewMatrix(target - direction * distOffMainCam, target);
}

HNCRSP_NAMESPACE_END