#include "Mesh.h"
#include "Debug.h"


void Mesh::ConstructMesh()
{
    if (!vertData.empty())
    {
        Debug::Warn("Mesh already constructed.");
        return;
    }
    else if (vertices.empty())
    {
        Debug::Error("No data to construct mesh.");
        return;
    }
    
    size_t vertArrayDataSize = vertices.size()*3 + colors.size()*3 + normals.size()*3 + uvs.size()*2;
    vertData.reserve(vertArrayDataSize);

    for (size_t vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
    {
        if (!vertices.empty())
        {
            vertData.push_back(vertices[vertIndex].x);
            vertData.push_back(vertices[vertIndex].y);
            vertData.push_back(vertices[vertIndex].z);
        }

        if (!colors.empty())
        {
            vertData.push_back(colors[vertIndex].x);
            vertData.push_back(colors[vertIndex].y);
            vertData.push_back(colors[vertIndex].z);
        }

        if (!uvs.empty())
        {
            vertData.push_back(uvs[vertIndex].x);
            vertData.push_back(uvs[vertIndex].y);
        }

        if (!normals.empty())
        {
            vertData.push_back(normals[vertIndex].x);
            vertData.push_back(normals[vertIndex].y);
            vertData.push_back(normals[vertIndex].z);
        }
    }

    // for (int i = 0; i < vertData.size(); i+=11)
    // {
    //     Debug::Log(fmt::format("[{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}]",
    //         vertData[i + 0],
    //         vertData[i + 1],
    //         vertData[i + 2],
    //         vertData[i + 3],
    //         vertData[i + 4],
    //         vertData[i + 5],
    //         vertData[i + 6],
    //         vertData[i + 7],
    //         vertData[i + 8],
    //         vertData[i + 9],
    //         vertData[i + 10]
    //     ));
    // }

    m_VAO.CreateVAO(
        vertData.data(),
        vertData.size() * sizeof(float),
        indices.data(),
        indices.size() * sizeof(GLuint), 
        GL_STATIC_DRAW
    );


    size_t numAttribElements = 
        3 * !vertices.empty() +
        3 * !colors.empty()   +
        2 * !uvs.empty()       +
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

    if (!uvs.empty())
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

Mesh::Mesh(Mesh&& other) noexcept
{
    vertices = std::move(other.vertices);
    colors = std::move(other.colors);
    normals = std::move(other.normals);
    uvs = std::move(other.uvs);
    indices = std::move(other.indices);

    vertData = std::move(other.vertData);
    m_VAO = std::move(other.m_VAO);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    vertices = std::move(other.vertices);
    colors = std::move(other.colors);
    normals = std::move(other.normals);
    uvs = std::move(other.uvs);
    indices = std::move(other.indices);

    vertData = std::move(other.vertData);
    m_VAO = std::move(other.m_VAO);

    return *this;
}

void Mesh::Bind() const
{
    m_VAO.Bind();
}

void Mesh::Draw(const Shader& shader)
{
    Debug::Log(m_VAO.getID());
    m_VAO.Bind();
    shader.Use();

    for (const glm::vec3& i_position : positions)
    {
        glm::mat4 modelMatrix = GetModelMatrix(i_position);
        shader.setMatrix3Uniform("u_normalMatrix", glm::mat4(glm::transpose(glm::inverse(modelMatrix))));
        shader.setMatrix4Uniform("u_model", modelMatrix);
        GLCall( glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0) );
    }
}

glm::mat4 Mesh::GetModelMatrix(const glm::vec3& position) const
{
    // maybe add rotation and scale

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    return model;
}

void Mesh::AddPosition(const glm::vec3& position)  // add batching
{
    positions.push_back(position);
}

VertexArray& Mesh::GetVAO()
{
    return m_VAO;
}