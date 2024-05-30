#pragma once

#include "src/pch/pch.h"
#include "src/Object.h"
#include "src/components/Transform.h"
#include "src/graphics/Light.h"


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
    float attenuation_linear = 0.7f;
    float attenuation_quadratic = 1.8f;

public:
    PointLight(const glm::vec3& position, const glm::vec3& color,
        float ambient, float diffuse, float specular);
        
    ~PointLight() = default;

    void ConfigureShader(const Shader* shader) const override final;

    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
};

HNCRSP_NAMESPACE_END