#include "pch/pch.h"

#include "Debug.h"
#include "Cube.h"
#include "Texture.h"


Cube::Cube()
{
    static_cast<Mesh*>(this)->vertices = m_vertices;
    static_cast<Mesh*>(this)->colors = m_colors;
    static_cast<Mesh*>(this)->normals = m_normals;
    static_cast<Mesh*>(this)->uv = m_uv;
    static_cast<Mesh*>(this)->indices = m_indices;
    ConstructMesh();

    shader = Shader(
        "../resources/shaders/vertex.vert",
        "../resources/shaders/fragment.frag"
    );
}

Cube::Cube(Cube&& other) noexcept
{
    positions = std::move(other.positions);
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);
}

Cube& Cube::operator=(Cube&& other) noexcept
{
    positions = std::move(other.positions);
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);

    return *this;
}

Cube::~Cube()
{
}

