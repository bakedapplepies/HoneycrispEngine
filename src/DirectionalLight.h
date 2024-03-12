#pragma once

#include "src/pch/pch.h"
#include "src/Object.h"


HNCRSP_NAMESPACE_START

class DirectionalLight : public Object
{
public:
    glm::vec3 direction;
    glm::vec3 colorEmit;

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

    glm::vec3& GetColor();    
};

HNCRSP_NAMESPACE_END