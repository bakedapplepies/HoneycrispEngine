#include "PointLight.h"


HNCRSP_NAMESPACE_START

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color)
    : position(position), colorEmit(color)
{
}

PointLight::PointLight(const PointLight& other)
{
    position = other.position;
    colorEmit = other.colorEmit;
}

PointLight& PointLight::operator=(const PointLight& other)
{
    position = other.position;
    colorEmit = other.colorEmit;

    return *this;
}

PointLight::PointLight(PointLight&& other) noexcept
{
    position = std::move(other.position);
    colorEmit = std::move(other.colorEmit);

    other.position = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
}

PointLight& PointLight::operator=(PointLight&& other) noexcept
{
    position = std::move(other.position);
    colorEmit = std::move(other.colorEmit);
    
    other.position = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);

    return *this;
}

glm::vec3& PointLight::GetColor()
{
    return colorEmit;
}

HNCRSP_NAMESPACE_END