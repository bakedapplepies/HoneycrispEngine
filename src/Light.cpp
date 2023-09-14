#include "pch/pch.h"

#include "Debug.h"
#include "Light.h"


Light::Light(const glm::vec3& color)
    : m_colorEmit(color)
{
}

Light::Light(Light&& other) noexcept
{
    positions = std::move(other.positions);
    m_colorEmit = other.m_colorEmit;
    this->GetVAO() = std::move(other.GetVAO());
}

Light& Light::operator=(Light&& other) noexcept
{
    positions = std::move(other.positions);
    m_colorEmit = other.m_colorEmit;
    this->GetVAO() = std::move(other.GetVAO());

    return *this;
}

glm::vec3& Light::GetColor()
{
    return m_colorEmit;
}