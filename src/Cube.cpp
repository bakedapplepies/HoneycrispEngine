#include "pch/pch.h"

#include "Debug.h"
#include "Cube.h"


Cube::Cube(const glm::vec3& position)
    : Mesh(vertices, std::vector<float>(0), normals, uv, indices)
{
    std::cout << "get cubed" << '\n';
    this->position = position;

    EnableVertexAttribPostion(true);
    EnableVertexAttribUV(true);
    EnableVertexAttribNormals(true);

    shader = Shader(
        "resources/shaders/vertex.vert",
        "resources/shaders/fragment.frag"
    );

    shader.Use();
    GLCall(glUniform1i(glGetUniformLocation(shader.getID(), "uTexture0"), 0));
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
    shader = std::move(other.shader);

    return *this;
}

Cube::~Cube()
{
    Delete();
}