#include "SpotLight.h"


HNCRSP_NAMESPACE_START

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color)
    : position(position), direction(direction), colorEmit(color)
{
}

SpotLight::SpotLight(const SpotLight& other)
{
    position = other.position;
    direction = other.direction;
    colorEmit = other.colorEmit;
}

SpotLight& SpotLight::operator=(const SpotLight& other)
{
    position = other.position;
    direction = other.direction;
    colorEmit = other.colorEmit;

    return *this;
}

SpotLight::SpotLight(SpotLight&& other) noexcept
{
    position = std::move(other.position);
    direction = std::move(other.direction);
    colorEmit = std::move(other.colorEmit);

    other.position = glm::vec3(0.0f);
    other.direction = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
}

SpotLight& SpotLight::operator=(SpotLight&& other) noexcept
{
    position = std::move(other.position);
    direction = std::move(other.direction);
    colorEmit = std::move(other.colorEmit);
    
    other.position = glm::vec3(0.0f);
    other.direction = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);

    return *this;
}

glm::vec3& SpotLight::GetColor()
{
    return colorEmit;
}

HNCRSP_NAMESPACE_END