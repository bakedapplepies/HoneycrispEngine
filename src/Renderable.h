#pragma once

#include "pch/pch.h"
#include "Object.h"
#include "core/Shader.h"


class Renderable : public Object
{
public:
    virtual void Draw(std::shared_ptr<Shader> shader) const = 0;
};