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
    Light() = default;
    Light(glm::vec3 colorEmit) : colorEmit(colorEmit) {}
    virtual ~Light() = default;

protected:
    friend Scene;
    virtual void ConfigureShader(const Shader* shaders) const = 0;
};

HNCRSP_NAMESPACE_END