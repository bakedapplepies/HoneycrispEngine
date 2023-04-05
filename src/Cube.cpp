#include "pch/pch.h"

#include "Debug.h"
#include "Cube.h"


Cube::Cube(int x, int y, int z)
{
    position = glm::vec3(x, y ,z);
    std::cout << '[' << this << "] position: " << &position << '\n';
    std::cout << '\t' << this->position.x << '\n';
    std::cout << '\t' << this->position.y << '\n';
    std::cout << '\t' << this->position.z << '\n';

    m_VAO.CreateVAO(m_vertices, sizeof(m_vertices), m_indicies, sizeof(m_indicies), GL_STATIC_DRAW);
    m_VAO.Bind();
    m_texture.CreateTexture("resources/textures/grass_block_side.png");

    // Vertex Attributes
    GLCall(glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    ));
    GLCall(glEnableVertexAttribArray(0));

    // Color RGB
    GLCall(glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    ));
    GLCall(glEnableVertexAttribArray(1));

    // Texture Coordinates XY (2D)
    GLCall(glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    ));
    GLCall(glEnableVertexAttribArray(2));
}

// Cube::~Cube() = default;

void Cube::Draw() const
{
    m_texture.Bind();
    m_VAO.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

glm::vec3 Cube::GetPosition() const
{
    return position;
}