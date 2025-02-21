#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/Shader.h"

HNCRSP_NAMESPACE_START

class Scene;
class ILight
{
public:
    glm::vec3 colorEmit;

public:
    ILight(glm::vec3 colorEmit) : colorEmit(colorEmit) {}
    virtual ~ILight() = default;

    ILight(const ILight& other) = delete;
    ILight(ILight&& other) noexcept = delete;
    ILight& operator=(const ILight& other) = delete;
    ILight& operator=(ILight&& other) noexcept = delete;

protected:
    friend Scene;
    virtual void ConfigureShader(const Shader* shader) const = 0;
};

HNCRSP_NAMESPACE_END