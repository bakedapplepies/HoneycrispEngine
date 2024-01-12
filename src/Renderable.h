#pragma once

#include "pch/pch.h"
#include "Object.h"
#include "core/Shader.h"
#include "types/Transform.h"


class Renderable : public Object
{
public:  // TODO: check accessibility
    std::vector<Transform> transforms;

public:
    virtual inline void addTransform(const Transform& transform) { transforms.push_back(transform); };
    virtual void Draw(std::shared_ptr<Shader> shader) const = 0;
    virtual ~Renderable() {}
};