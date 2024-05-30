#include "src/pch/pch.h"

#include "DepthPassCamera.h"


HNCRSP_NAMESPACE_START

DepthPassCamera::DepthPassCamera(const glm::vec3& initialPos)
{
    position = initialPos;
}

void DepthPassCamera::SetPos(const glm::vec3& newPos)
{
    position = newPos;
}

void DepthPassCamera::ChangePos(const glm::vec3& posOffset)
{
    position += posOffset;
}

void DepthPassCamera::SetDirection(const glm::vec3& newDirection)
{
    direction = newDirection;
}

glm::mat4 DepthPassCamera::GetViewMatrix(const glm::vec3& lightPos, const glm::vec3& target) const
{
    return glm::lookAt(
        lightPos,
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

HNCRSP_NAMESPACE_END