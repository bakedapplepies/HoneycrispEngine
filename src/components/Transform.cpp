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

    other.position = glm::vec3(0.0f);
    other.eulerAngles = glm::vec3(0.0f);
    other.scale = glm::vec3(0.0f);
}

Transform& Transform::operator=(Transform&& other) noexcept
{
    position = std::move(other.position);
    eulerAngles = std::move(other.eulerAngles);
    scale = std::move(other.scale);

    other.position = glm::vec3(0.0f);
    other.eulerAngles = glm::vec3(0.0f);
    other.scale = glm::vec3(0.0f);
    
    return *this;
}