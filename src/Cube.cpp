#include "pch/pch.h"

#include "Debug.h"
#include "Cube.h"


Cube::Cube(const glm::vec3& position)
{
    this->position = position;
    vertices = m_vertices;
    colors = m_colors;
    normals = m_normals;
    uv = m_uv;
    indices = m_indices;
    ConstructMesh();

    EnableVertexAttribPostion(true);
    EnableVertexAttribColor(true);
    EnableVertexAttribUV(true);
    EnableVertexAttribNormals(true);

    shader = Shader(
        "../resources/shaders/vertex.vert",
        "../resources/shaders/fragment.frag"
    );

    shader.Use();
    GLCall(glUniform1i(glGetUniformLocation(shader.getID(), "uTexture0"), 0));

    std::cout << grassTopCoords.tl.x << ' ' << grassTopCoords.tl.y << '\n';
    std::cout << grassTopCoords.tr.x << ' ' << grassTopCoords.tr.y << '\n';
    std::cout << grassTopCoords.br.x << ' ' << grassTopCoords.br.y << '\n';
    std::cout << grassTopCoords.bl.x << ' ' << grassTopCoords.bl.y << '\n';
}

Cube::Cube(Cube&& other) noexcept
{
    std::cout << "Move constructor of Cube called." << '\n';

    position = other.position;
    shader = std::move(other.shader);
}

Cube& Cube::operator=(Cube&& other) noexcept
{
    std::cout << "Move assignment operator of Cube called." << '\n';

    position = other.position;
    this->GetVAO() = std::move(other.GetVAO());
    shader = std::move(other.shader);

    return *this;
}

Cube::~Cube()
{
    std::cout << "Deleting cube" << '\n';
}

