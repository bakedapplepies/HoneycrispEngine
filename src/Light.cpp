#include "pch/pch.h"

#include "Debug.h"
#include "Light.h"


Light::Light(const glm::vec3& color)
    : m_colorEmit(color)
{
    InitializeAttributeData();
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
    m_colorEmit = other.m_colorEmit;
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);
}

Light& Light::operator=(Light&& other) noexcept
{
    positions = std::move(other.positions);
    m_colorEmit = other.m_colorEmit;
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);

    return *this;
}

Light::~Light()
{
}

glm::vec3& Light::GetColor()
{
    return m_colorEmit;
}

void Light::InitializeAttributeData()
{
    static_cast<Mesh*>(this)->vertices = {
        // front
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // right
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,

        // back
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,

        // left
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,

        // top
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // bottom
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
    };

    static_cast<Mesh*>(this)->colors = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
    };

    static_cast<Mesh*>(this)->indices = {
        // front
        0, 1, 2,
        0, 2, 3,

        // right
        4, 5, 6,
        4, 6, 7,

        // back
        8, 9, 10,
        8, 10, 11,

        // left
        12, 13, 14,
        12, 14, 15,

        // top
        16, 17, 18,
        16, 18, 19,

        // back
        20, 21, 22,
        20, 22, 23
    };
}