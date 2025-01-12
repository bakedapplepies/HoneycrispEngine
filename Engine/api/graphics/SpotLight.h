#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/ILight.h"

HNCRSP_NAMESPACE_START

class SpotLight : public ILight
{
public:
    // glm::vec3 colorEmit;  In Light interface
    glm::vec3 position;
    glm::vec3 direction;

    float ambient;
    float diffuse;
    float specular;

    float attenuation_constant = 1.0f;
    float attenuation_linear = 0.007f;
    float attenuation_quadratic = 0.0045f;

    float cutOff = glm::cos(glm::radians(15.0f));
    float outerCutOff = glm::cos(glm::radians(25.0f));

public:
    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color,
        float ambient, float diffuse, float specular);

    ~SpotLight() = default;

    void ConfigureShader(const Shader* shader) const override final;

    glm::vec3 GetAmbient() const;
    glm::vec3 GetDiffuse() const;
    glm::vec3 GetSpecular() const;
};

HNCRSP_NAMESPACE_END