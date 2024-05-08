#pragma once

#include "src/pch/pch.h"
#include "src/Object.h"
#include "src/components/Transform.h"
#include "src/Light.h"


HNCRSP_NAMESPACE_START

class PointLight : public Light
{
public:
    // glm::vec3 colorEmit;  In Light interface
    glm::vec3 position;

    float ambient;
    float diffuse;
    float specular;

    float attenuation_constant = 1.0f;
    float attenuation_linear = 0.001f;
    float attenuation_quadratic = 0.0002f;

public:
    PointLight(const glm::vec3& position, const glm::vec3& color,
        float ambient, float diffuse, float specular);
        
    PointLight(const PointLight& other);
    PointLight(PointLight&& other) noexcept;
    PointLight& operator=(const PointLight& other);
    PointLight& operator=(PointLight&& other) noexcept;
    ~PointLight() = default;

    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
};

HNCRSP_NAMESPACE_END