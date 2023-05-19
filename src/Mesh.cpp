#include "Mesh.h"
#include "Debug.h"


Mesh::Mesh(
    const std::vector<float>& vertices,
    const std::vector<float>& colors,
    const std::vector<float>& normals,
    const std::vector<float>& uv,
    const std::vector<unsigned int>& indices
) : vertices(vertices), colors(colors), normals(normals), uv(uv), indices(indices)
{
    std::cout << vertices.size() << '\n';
    std::cout << "Mesh Constructor called." << '\n';
    ConstructMesh();
}

Mesh::~Mesh()
{
}

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
        currentOffset += 3;
    }

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
        currentOffset += 3;
    }

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
        currentOffset += 2;
    }

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
        currentOffset += 3;
    }
}

void Mesh::EnableVertexAttribPostion(bool on) const
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

void Mesh::Draw() const
{
    // maybe add use shader here
    m_VAO.Bind();
    GLCall( glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0) );
}

glm::vec3& Mesh::GetPosition()
{
    return position;
}

glm::mat4 Mesh::GetModelMatrix() const
{
    // maybe add rotation and scale

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    return model;
}

Shader& Mesh::GetShader()
{
    return shader;
}

VertexArray& Mesh::GetVAO()
{
    return m_VAO;
}