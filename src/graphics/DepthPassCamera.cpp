#include "src/pch/hncrsp_pch.h"

#include "DepthPassCamera.h"


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
    direction = new_direction;
}

glm::mat4 DepthPassCamera::GetViewMatrix(const glm::vec3& light_pos, const glm::vec3& target) const
{
    return glm::lookAt(
        light_pos,
        target,
        cameraUp
    );
}

glm::mat4 DepthPassCamera::GetProjectionMatrix() const
{
    return glm::ortho(
        -10.0f,  // left
         10.0f,  // right
        -10.0f,  // bottom
         10.0f,  // top,
        nearClippingPlane,
        farClippingPlane
    );
}

glm::mat4 DepthPassCamera::GetViewProjectionMatrix(const glm::vec3& light_pos, const glm::vec3& target) const
{
    return GetProjectionMatrix() * GetViewMatrix(light_pos, target);
}

HNCRSP_NAMESPACE_END