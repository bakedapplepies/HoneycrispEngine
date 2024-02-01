#pragma once

#include "src/pch/pch.h"
#include "Object.h"


HNCRSP_NAMESPACE_START

class SpotLight : public Object
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 colorEmit;

public:
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color);
    SpotLight(const SpotLight& other);
    SpotLight(SpotLight&& other) noexcept;
    SpotLight& operator=(const SpotLight& other);
    SpotLight& operator=(SpotLight&& other) noexcept;
    ~SpotLight() = default;

    void OnUpdate() override {}

    glm::vec3& GetColor();
};

HNCRSP_NAMESPACE_END