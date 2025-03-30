#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/Mesh.h"

HNCRSP_NAMESPACE_START

class Billboard : public IRenderable
{
private:
    Mesh m_mesh;

public:
    Billboard(uint32_t width, uint32_t height);

    void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID, const Material& material) const override final;
};

HNCRSP_NAMESPACE_END