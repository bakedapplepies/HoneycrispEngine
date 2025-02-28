#include "api/components/Transform.h"

HNCRSP_NAMESPACE_START

void translate(Transform* transform, const glm::vec3& vec)
{
    transform->position += vec;
}

void scale(Transform* transform, float multiplier)
{
    transform->scale *= multiplier;
}

void rotate(Transform* transform, const glm::vec3& rXYZ)
{
    transform->eulerAngles += rXYZ;
}

HNCRSP_NAMESPACE_END