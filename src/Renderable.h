#pragma once

#include "src/pch/pch.h"
#include "Object.h"
#include "core/Shader.h"
#include "types/Transform.h"


class Renderable : public Object
{
public:  // TODO: check accessibility
    std::vector<Transform> transforms;

protected:
    glm::vec3 m_relativeOrigin = glm::vec3(0.0f);

public:
    inline glm::vec3 getRelativeOrigin() const
    {
        return m_relativeOrigin;
    }

    inline void setRelativeOrigin(const glm::vec3& relOrigin)
    {
        m_relativeOrigin = relOrigin;
    }

    virtual inline void addTransform(const Transform& transform)
    {
        transforms.push_back(transform);
    }

    virtual void Draw(std::shared_ptr<Shader> shader) const = 0;
    virtual ~Renderable() {}
};