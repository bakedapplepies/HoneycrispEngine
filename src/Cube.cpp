#include "pch/pch.h"

#include "Debug.h"
#include "Cube.h"


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
    GLCall(glUniform1i(glGetUniformLocation(shader.getID(), "uTexture0"), 0));
}

Cube::Cube(Cube&& other) noexcept
{
    std::cout << "Move constructor of Cube called." << '\n';

    positions = std::move(other.positions);
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);
}

Cube& Cube::operator=(Cube&& other) noexcept
{
    std::cout << "Move assignment operator of Cube called." << '\n';

    positions = std::move(other.positions);
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);

    return *this;
}

Cube::~Cube()
{
}

