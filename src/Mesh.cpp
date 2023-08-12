#include "Mesh.h"
#include "Debug.h"


void Mesh::ConstructMesh()
{
    unsigned int vertArrayDataSize = vertices.size() + colors.size() + normals.size() + uv.size();
    vertData.reserve(vertArrayDataSize);

    for (int vertIndex = 0; vertIndex < vertices.size()/3; vertIndex++)
    {
        if (!vertices.empty())
        {
            vertData.push_back(vertices[vertIndex*3 + 0]);
            vertData.push_back(vertices[vertIndex*3 + 1]);
            vertData.push_back(vertices[vertIndex*3 + 2]);
        }

        if (!colors.empty())
        {
            vertData.push_back(colors[vertIndex*3 + 0]);
            vertData.push_back(colors[vertIndex*3 + 1]);
            vertData.push_back(colors[vertIndex*3 + 2]);
        }

        if (!uv.empty())
        {
            vertData.push_back(uv[vertIndex*2 + 0]);
            vertData.push_back(uv[vertIndex*2 + 1]);
        }

        if (!normals.empty())
        {
            vertData.push_back(normals[vertIndex*3 + 0]);
            vertData.push_back(normals[vertIndex*3 + 1]);
            vertData.push_back(normals[vertIndex*3 + 2]);
        }
    }

    m_VAO.CreateVAO(
        vertData.data(),
        vertData.size() * sizeof(float),
        indices.data(),
        indices.size() * sizeof(unsigned int), 
        GL_STATIC_DRAW
    );


    unsigned int numAttribElements = 
        3 * !vertices.empty() +
        3 * !colors.empty()   +
        2 * !uv.empty()       +
        3 * !normals.empty();
    unsigned int currentOffset = 0;

    // Postions XYZ
    if (!vertices.empty())
    {
        GLCall(glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            numAttribElements * sizeof(float),
            (void*)(currentOffset * sizeof(float))
        ));
        EnableVertexAttribPosition(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribPosition(false); }

    // Color RGB
    if (!colors.empty())
    {
        GLCall(glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            numAttribElements * sizeof(float),
            (void*)(currentOffset * sizeof(float))
        ));
        EnableVertexAttribColor(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribColor(false); }

    if (!uv.empty())
    {
        // Texture Coordinates XY
        GLCall(glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            numAttribElements * sizeof(float),
            (void*)(currentOffset * sizeof(float))
        ));
        EnableVertexAttribUV(true);
        currentOffset += 2;
    }
    else { EnableVertexAttribUV(false); }

    if (!normals.empty())
    {
        // Normals XYZ (Normalized vectors)
        GLCall(glVertexAttribPointer(
            3,
            3,
            GL_FLOAT, 
            GL_FALSE,
            numAttribElements * sizeof(float),
            (void*)(currentOffset * sizeof(float))
        ));
        EnableVertexAttribNormals(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribNormals(false); }
}

void Mesh::EnableVertexAttribPosition(bool on) const
{
    // Becareful of these binds
    m_VAO.Bind();
    if (on)
    {
        GLCall(glEnableVertexAttribArray(0));
    }
    else
    {
        GLCall(glDisableVertexAttribArray(0));
    }
}

void Mesh::EnableVertexAttribColor(bool on) const
{
    m_VAO.Bind();
    if (on)
    {
        GLCall(glEnableVertexAttribArray(1));
    }
    else
    {
        GLCall(glDisableVertexAttribArray(1));
    }
}

void Mesh::EnableVertexAttribUV(bool on) const
{
    m_VAO.Bind();
    if (on)
    {
        GLCall(glEnableVertexAttribArray(2));
    }
    else
    {
        GLCall(glDisableVertexAttribArray(2));
    }
}

void Mesh::EnableVertexAttribNormals(bool on) const
{
    m_VAO.Bind();
    if (on)
    {
        GLCall(glEnableVertexAttribArray(3));
    }
    else
    {
        GLCall(glDisableVertexAttribArray(3));
    }
}

void Mesh::Bind()
{
    m_VAO.Bind();
}

void Mesh::Draw()
{
    m_VAO.Bind();
    GLCall( glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0) );
}

glm::mat4 Mesh::GetModelMatrix(const glm::vec3& position) const
{
    // maybe add rotation and scale

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    return model;
}

// Shader& Mesh::GetShader()
// {
//     return shader;
// }

VertexArray& Mesh::GetVAO()
{
    return m_VAO;
}