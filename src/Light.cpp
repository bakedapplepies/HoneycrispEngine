#include "pch/pch.h"

#include "Debug.h"
#include "Light.h"


Light::Light(const glm::vec3& color)
{
    vertices = m_vertices;
    colors = m_colors;
    indices = m_indices;
    ConstructMesh();

    EnableVertexAttribUV(false);
    EnableVertexAttribNormals(false);

    shader = Shader(
        "../resources/shaders/lightvertex.vert",
        "../resources/shaders/lightfragment.frag"
    );
}

Light::Light(Light&& other) noexcept
{
    positions = std::move(other.positions);
    m_color = other.m_color;
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);
}

Light& Light::operator=(Light&& other) noexcept
{
    positions = std::move(other.positions);
    m_color = other.m_color;
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);

    return *this;
}

Light::~Light()
{
}

glm::vec3& Light::GetColor()
{
    return m_color;
}