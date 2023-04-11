#include "pch/pch.h"

#include "Debug.h"
#include "Light.h"


Light::Light(const glm::vec3& position, const glm::vec3& color)
    : m_position(position), m_color(color)
{
    m_VAO.CreateVAO(m_vertices, sizeof(m_vertices), m_indicies, sizeof(m_indicies), GL_STATIC_DRAW);
    m_VAO.Bind();
    m_shader.CreateShader(
        "resources/shaders/lightvertex.vert",
        "resources/shaders/lightfragment.frag"
    );

    // Vertex Attributes
    GLCall(glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    ));
    GLCall(glEnableVertexAttribArray(0));

    // Color
    GLCall(glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    ));
    GLCall(glEnableVertexAttribArray(1));
}

Light::~Light()
{
    m_VAO.Delete();
    m_shader.Delete();
}

void Light::Draw() const
{
    m_VAO.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

glm::vec3* Light::GetPosition()
{
    return &m_position;
}

glm::vec3* Light::GetColor()
{
    return &m_color;
}

glm::mat4 Light::GetModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    return model;
}

Shader* Light::GetShader()
{
    return &m_shader;
}