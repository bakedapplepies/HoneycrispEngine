#pragma once

#include "pch/pch.h"

#include "NonRenderable.h"


class Light : public NonRenderable
{
private:
    glm::vec3 m_colorEmit;

public:
    Light() = default;
    Light(const glm::vec3& color);
    Light(const Light& other) = delete;
    Light(Light&& other) noexcept;
    Light& operator=(const Light& other) = delete;
    Light& operator=(Light&& other) noexcept;
    ~Light() = default;

    void OnUpdate() {}

    glm::vec3& GetColor();
};