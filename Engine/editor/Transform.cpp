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

HNCRSP_NAMESPACE_END