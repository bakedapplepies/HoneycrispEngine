#pragma once

#include "src/pch/pch.h"
#include "Object.h"
#include "types/Transform.h"


HNCRSP_NAMESPACE_START

class PointLight : public Object
{
public:
    glm::vec3 position;
    glm::vec3 colorEmit;

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

    void OnUpdate() override {}

    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
};

HNCRSP_NAMESPACE_END