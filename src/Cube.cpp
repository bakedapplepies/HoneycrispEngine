#include "pch/pch.h"

#include "Debug.h"
#include "Cube.h"
#include "Texture.h"


Cube::Cube()
{
    vertices = m_vertices;
    colors = m_colors;
    normals = m_normals;
    uv = m_uv;
    indices = m_indices;
    ConstructMesh();

    shader = Shader(
        "../resources/shaders/vertex.vert",
        "../resources/shaders/fragment.frag"
    );

    shader.Use();
    shader.setIntUniform("uTexture", Textures::mainTextureMap.getTextureUnit());
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

