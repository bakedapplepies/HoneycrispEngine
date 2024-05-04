#pragma once

#include "src/pch/pch.h"
#include "src/Object.h"


HNCRSP_NAMESPACE_START

class SpotLight : public Object
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 colorEmit;

    float ambient;
    float diffuse;
    float specular;

    float attenuation_constant = 1.0f;
    float attenuation_linear = 0.007f;
    float attenuation_quadratic = 0.0045f;

public:
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color,
        float ambient, float diffuse, float specular);

    SpotLight(const SpotLight& other);
    SpotLight(SpotLight&& other) noexcept;
    SpotLight& operator=(const SpotLight& other);
    SpotLight& operator=(SpotLight&& other) noexcept;
    ~SpotLight() = default;

    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
};

HNCRSP_NAMESPACE_END