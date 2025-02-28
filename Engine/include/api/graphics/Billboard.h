#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/scene/IRenderable.h"
#include "api/graphics/core/VertexArray.h"
#include "api/graphics/core/Texture2D.h"

HNCRSP_NAMESPACE_START

class Billboard : public IRenderable
{
private:
    VertexArray m_VAO;

public:
    Billboard(uint32_t width, uint32_t height);

    void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID, const Material& material) const override final;
};

HNCRSP_NAMESPACE_END