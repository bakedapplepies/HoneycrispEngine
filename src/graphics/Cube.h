#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/ecs/ECSManager.h"
#include "src/Renderable.h"
#include "src/graphics/core/VertexArray.h"


HNCRSP_NAMESPACE_START

class Cube : public Renderable
{
private:
    VertexArray m_VAO;

public:
    Cube();
    Cube(const Cube& other) = delete;
    Cube& operator=(const Cube& other) = delete;
    Cube(Cube&& other) noexcept;
    Cube& operator=(Cube&& other) noexcept;
    ~Cube() = default;
    
    void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID) const override final;

private:
    void _InitializeAttributeData();
};

HNCRSP_NAMESPACE_END