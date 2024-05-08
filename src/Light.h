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
    ~Light() = default;

protected:
    void AddShaders();
};

HNCRSP_NAMESPACE_END