#pragma once

#include <Envy/EnvyInstance.h>

#include "../Renderer.h"

class Quad
{
public:
    Quad(const Envy::EnvyInstance* envy_instance);
    ~Quad() = default;

    RenderCommand GetRenderCmd() const;

public:
    Material material;
    Transform transform;

private:
    GLResource<Envy::VertexArray> m_vao;
};