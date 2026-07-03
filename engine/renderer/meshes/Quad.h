#pragma once

#include <Envy/EnvyInstance.h>

#include "renderer/Renderer.h"

HNCRSP_NAMESPACE_START

class Quad
{
public:
    Quad(const Envy::EnvyInstance* envy_instance);
    ~Quad() = default;

    RenderCommand GetRenderCmd() const;

public:
    Material material;

private:
    GLResource<Envy::VertexArray> m_vao;
};

HNCRSP_NAMESPACE_END