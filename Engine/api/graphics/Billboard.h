#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/scene/Renderable.h"
#include "api/graphics/core/VertexArray.h"
#include "api/graphics/core/Texture2D.h"

HNCRSP_NAMESPACE_START

class Billboard : public Renderable
{
private:
    VertexArray m_VAO;
    const Texture2D* m_texture;

public:
    Billboard(uint32_t width, uint32_t height, const Texture2D* texture);

    void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID) const override final;
};

HNCRSP_NAMESPACE_END