#include "pch/pch.h"

#include "utils/Debug.h"
#include "Light.h"


Light::Light(const glm::vec3& color)
    : m_colorEmit(color)
{
}

Light::Light(Light&& other) noexcept
{
    positions = std::move(other.positions);
    m_colorEmit = other.m_colorEmit;
}

Light& Light::operator=(Light&& other) noexcept
{
    positions = std::move(other.positions);
    m_colorEmit = other.m_colorEmit;

    return *this;
}

glm::vec3& Light::GetColor()
{
    return m_colorEmit;
}