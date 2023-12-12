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