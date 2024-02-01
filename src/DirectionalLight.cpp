#include "DirectionalLight.h"


HNCRSP_NAMESPACE_START

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color)
    : direction(direction), colorEmit(color)
{
}

DirectionalLight::DirectionalLight(const DirectionalLight& other)
{
    direction = other.direction;
    colorEmit = other.colorEmit;
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& other)
{
    direction = other.direction;
    colorEmit = other.colorEmit;

    return *this;
}

DirectionalLight::DirectionalLight(DirectionalLight&& other) noexcept
{
    direction = std::move(other.direction);
    colorEmit = std::move(other.colorEmit);

    other.direction = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
}

DirectionalLight& DirectionalLight::operator=(DirectionalLight&& other) noexcept
{
    direction = std::move(other.direction);
    colorEmit = std::move(other.colorEmit);
    
    other.direction = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);

    return *this;
}

glm::vec3& DirectionalLight::GetColor()
{
    return colorEmit;
}

HNCRSP_NAMESPACE_END