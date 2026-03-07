#pragma once

#include <Envy/opengl/Texture2D.h>
#include <Envy/opengl/Pipeline.h>

#include "core/Types.h"

class Material
{
public:
    Material();
    ~Material();

    GLResource<Envy::Texture2D> albedo;
    GLResource<Envy::Pipeline> pipeline;
};