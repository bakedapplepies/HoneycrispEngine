#pragma once

#include "src/pch/pch.h"
#include "src/graphics/core/Shader.h"


HNCRSP_NAMESPACE_START

class Scene;
class Light
{
public:
    glm::vec3 colorEmit;

public:
    Light(glm::vec3 colorEmit) : colorEmit(colorEmit) {}
    virtual ~Light() = default;

    Light(const Light& other) = delete;
    Light(Light&& other) noexcept = delete;
    Light& operator=(const Light& other) = delete;
    Light& operator=(Light&& other) noexcept = delete;

protected:
    friend Scene;
    virtual void ConfigureShader(const Shader* shaders) const = 0;
};

HNCRSP_NAMESPACE_END