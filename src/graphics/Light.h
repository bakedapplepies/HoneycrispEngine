#pragma once

#include "src/pch/pch.h"
#include "src/graphics/core/Shader.h"


HNCRSP_NAMESPACE_START

class Light
{
private:
    // std::vector< std::shared_ptr<Shader> > m_shaders;

public:
    glm::vec3 colorEmit;

public:
    Light() = default;
    Light(glm::vec3 colorEmit) : colorEmit(colorEmit) {}
    virtual ~Light() = default;
    // void AddShaders(const std::vector< std::shared_ptr<Shader> >& shaders);
    virtual void ConfigureShader(const Shader* shaders) = 0;
};

HNCRSP_NAMESPACE_END