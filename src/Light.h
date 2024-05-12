#pragma once

#include "src/pch/pch.h"
#include "src/graphics/core/Shader.h"


HNCRSP_NAMESPACE_START

class Light
{
private:
    std::vector< std::shared_ptr<Shader> > m_shaders;

public:
    glm::vec3 colorEmit;

public:
    Light() = default;
    Light(glm::vec3 colorEmit) : colorEmit(colorEmit) {}
    virtual ~Light() = default;

protected:
    virtual void AddShaders(const std::vector<Shader>& shaders) = 0;
};

HNCRSP_NAMESPACE_END