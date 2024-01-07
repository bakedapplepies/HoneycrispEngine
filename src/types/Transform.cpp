#include "Transform.h"

Transform::Transform(const Transform& other)
{
    position = other.position;
    eulerAngles = other.eulerAngles;
    scale = other.scale;
}
Transform& Transform::operator=(const Transform& other)
{
    position = other.position;
    eulerAngles = other.eulerAngles;
    scale = other.scale;

    return *this;
}

Transform::Transform(Transform&& other) noexcept
{
    position = std::move(other.position);
    eulerAngles = std::move(other.eulerAngles);
    scale = std::move(other.scale);
}

Transform& Transform::operator=(Transform&& other) noexcept
{
    position = std::move(other.position);
    eulerAngles = std::move(other.eulerAngles);
    scale = std::move(other.scale);

    return *this;
}