#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/graphics/Light.h"


HNCRSP_NAMESPACE_START

class DirectionalLight : public Light
{
public:
    // glm::vec3 colorEmit;  In Light interface
    glm::vec3 direction;

    float ambient;
    float diffuse;
    float specular;

public:
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color,
        float ambient, float diffuse, float specular);

    DirectionalLight(const DirectionalLight& other);
    DirectionalLight(DirectionalLight&& other) noexcept;
    DirectionalLight& operator=(const DirectionalLight& other);
    DirectionalLight& operator=(DirectionalLight&& other) noexcept;
    ~DirectionalLight() = default;

    void ConfigureShader(const Shader* shader) const override final;

    glm::vec3 GetAmbient() const;
    glm::vec3 GetDiffuse() const;
    glm::vec3 GetSpecular() const;
};

HNCRSP_NAMESPACE_END