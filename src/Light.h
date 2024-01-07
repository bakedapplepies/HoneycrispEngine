#pragma once

#include "pch/pch.h"
#include "Object.h"
#include "types/Transform.h"


class Light : public Object
{
private:
    glm::vec3 m_colorEmit;
    Transform transform;

public:
    Light(const glm::vec3& color, const Transform& transform);
    Light(const Light& other) = delete;
    Light(Light&& other) noexcept;
    Light& operator=(const Light& other) = delete;
    Light& operator=(Light&& other) noexcept;
    ~Light() = default;

    void onUpdate() {}

    glm::vec3& GetColor();
};