#include "Mesh.h"


HNCRSP_NAMESPACE_START

Mesh::Mesh()
{
    m_VAO = std::make_shared<VertexArray>();
}

void Mesh::ConstructMesh()
{
    if (!m_vertData.empty())
    {
        HNCRSP_LOG_WARN("Mesh already constructed.");
        return;
    }
    else if (vertices.empty())
    {
        HNCRSP_TERMINATE("No data to construct mesh.");
    }

    if (indices.empty())
    {
        HNCRSP_TERMINATE("No indices to draw mesh.");
    }
    
    size_t vertArrayDataSize = vertices.size()*3 + vertices.size()*3 + normals.size()*3 + uvs.size()*2;
    m_vertData.reserve(vertArrayDataSize);

    for (size_t vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
    {
        if (!vertices.empty())
        {
            m_vertData.push_back(vertices[vertIndex].x);
            m_vertData.push_back(vertices[vertIndex].y);
            m_vertData.push_back(vertices[vertIndex].z);
        }

        if (colors.empty())
        {
            m_vertData.push_back(1.0f);
            m_vertData.push_back(1.0f);
            m_vertData.push_back(1.0f);
        }
        else
        {
            m_vertData.push_back(colors[vertIndex].x);
            m_vertData.push_back(colors[vertIndex].y);
            m_vertData.push_back(colors[vertIndex].z);
        }

        if (!uvs.empty())
        {
            m_vertData.push_back(uvs[vertIndex].x);
            m_vertData.push_back(uvs[vertIndex].y);
        }

        if (!normals.empty())
        {
            m_vertData.push_back(normals[vertIndex].x);
            m_vertData.push_back(normals[vertIndex].y);
            m_vertData.push_back(normals[vertIndex].z);
        }
    }

    m_VAO->CreateVAO(
        m_vertData.data(),
        m_vertData.size() * sizeof(float),
        indices.data(),
        indices.size() * sizeof(GLuint), 
        GL_STATIC_DRAW
    );


    size_t numAttribElements = 
        3 +
        3 +
        2 * !uvs.empty()       +
        3 * !normals.empty();
    unsigned int currentOffset = 0;

    // Postions XYZ
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

    // Color RGB
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

    m_modelMatrix = glm::mat4(1.0f);
}

void Mesh::EnableVertexAttribPosition(bool on) const
{
    // Becareful of these binds
    m_VAO->Bind();
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
    m_VAO->Bind();
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
    m_VAO->Bind();
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
    m_VAO->Bind();
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
    transforms = std::move(other.transforms);

    vertices = std::move(other.vertices);
    colors = std::move(other.colors);
    normals = std::move(other.normals);
    uvs = std::move(other.uvs);
    indices = std::move(other.indices);
    textures = std::move(other.textures);

    m_vertData = std::move(other.m_vertData);
    m_VAO = std::move(other.m_VAO);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    transforms = std::move(other.transforms);

    vertices = std::move(other.vertices);
    colors = std::move(other.colors);
    normals = std::move(other.normals);
    uvs = std::move(other.uvs);
    indices = std::move(other.indices);
    textures = std::move(other.textures);

    m_vertData = std::move(other.m_vertData);
    m_VAO = std::move(other.m_VAO);

    return *this;
}

void Mesh::Draw(std::shared_ptr<Shader> shader) const
{
    m_VAO->Bind();
    shader->Use();

    for (unsigned int i = 0; i < textures.size(); i++)  // TODO: Fix repetition
    {
        textures[i]->Bind();
        if (textures[i]->getTextureType() == ETextureType::DIFFUSE)
        {
            shader->setIntUnf("u_material.albedo", textures[i]->getTextureUnit());
        }
        else if (textures[i]->getTextureType() == ETextureType::SPECULAR)
        {
            shader->setIntUnf("u_material.specular", textures[i]->getTextureUnit());
        }
    }

    for (const Transform& i_transform : transforms)
    {
        glm::mat4 modelMatrix = GetModelMatrix(i_transform);
        shader->setMat3Unf("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix))));
        shader->setMat4Unf("u_model", modelMatrix);
        GLCall( glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0) );
    }
}

glm::mat4 Mesh::GetModelMatrix(const Transform& transform) const
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, transform.position + m_relativeOrigin);
    
    glm::quat quaternion = glm::quat(transform.eulerAngles);
    glm::mat4 rotationMatrix = glm::toMat4(quaternion);
    modelMatrix *= rotationMatrix;

    modelMatrix = glm::scale(modelMatrix, transform.scale);

    return modelMatrix;
}

std::weak_ptr<VertexArray> Mesh::GetVAO()
{
    return std::weak_ptr<VertexArray>(m_VAO);
}

HNCRSP_NAMESPACE_END