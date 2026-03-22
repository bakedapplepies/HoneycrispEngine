#pragma once

#include <Envy/opengl/Texture2D.h>
#include <Envy/opengl/Pipeline.h>

#include "core/Defines.h"
#include "core/Types.h"

HNCRSP_NAMESPACE_START

class Material
{
public:
    Material();
    ~Material();

    GLResource<Envy::Texture2D> albedo;
    GLResource<Envy::Texture2D> normal;
    GLResource<Envy::Texture2D> roughness;
    GLResource<Envy::Pipeline> pipeline;
};

HNCRSP_NAMESPACE_END