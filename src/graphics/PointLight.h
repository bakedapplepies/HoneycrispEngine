#pragma once

#include "src/pch/hncrsp_pch.h"
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

    glm::vec3 GetAmbient() const;
    glm::vec3 GetDiffuse() const;
    glm::vec3 GetSpecular() const;
};

HNCRSP_NAMESPACE_END