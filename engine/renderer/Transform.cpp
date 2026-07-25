#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

HNCRSP_NAMESPACE_START

glm::mat4 Transform::GetModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    // rotation here
    model = glm::scale(model, scale);

    return model;
}

glm::mat3 Transform::GetNormalMatrix() const
{
    return glm::mat3(glm::transpose(glm::inverse(GetModelMatrix())));
}

HNCRSP_NAMESPACE_END