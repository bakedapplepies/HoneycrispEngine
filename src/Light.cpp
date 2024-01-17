#include "src/pch/pch.h"


#include "Light.h"


Light::Light(const glm::vec3& color, const Transform& transform)
    : m_colorEmit(color), transform(transform)
{
}

Light::Light(Light&& other) noexcept
{
    transform = std::move(other.transform);
    m_colorEmit = other.m_colorEmit;
}

Light& Light::operator=(Light&& other) noexcept
{
    transform = std::move(other.transform);
    m_colorEmit = other.m_colorEmit;

    return *this;
}

glm::vec3& Light::GetColor()
{
    return m_colorEmit;
}