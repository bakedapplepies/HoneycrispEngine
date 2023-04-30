#include "pch/pch.h"

#include "Debug.h"
#include "Cube.h"


Cube::Cube(const glm::vec3& position)
    : m_position(position)
{
    BuildMeshData();
    cubeMesh.ConstructMesh();
    m_shader.CreateShader(
        "resources/shaders/vertex.vert",
        "resources/shaders/fragment.frag"
    );

    cubeMesh.GetVAO().Bind();
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

    // Texture Coordinates XY (2D)
    GLCall(glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    ));
    GLCall(glEnableVertexAttribArray(2));

    GLCall(glVertexAttribPointer(
        3,
        3,
        GL_FLOAT, 
        GL_FALSE,
        8 * sizeof(float),
        (void*)(5 * sizeof(float))
    ));
    GLCall(glEnableVertexAttribArray(3));
}

Cube::~Cube()
{
    cubeMesh.Delete();
    m_shader.Delete();
}

void Cube::BuildMeshData()
{

}

void Cube::Draw() const
{
    GLCall(glUniform1i(glGetUniformLocation(m_shader.getID(), "uTexture0"), 0));
    cubeMesh.GetVAO().Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
    // glDrawElements(GL_TRIANGLES, );
}

glm::vec3& Cube::GetPosition()
{
    return m_position;
}

glm::mat4 Cube::GetModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    return model;
}

Shader& Cube::GetShader()
{
    return m_shader;
}