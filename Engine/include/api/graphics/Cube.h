#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/ecs/ECSManager.h"
#include "api/scene/IRenderable.h"
#include "api/graphics/Material.h"

HNCRSP_NAMESPACE_START

class Cube : public IRenderable
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
    
    void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID, const Material& material) const override final;

private:
    void _InitializeAttributeData();
};

HNCRSP_NAMESPACE_END