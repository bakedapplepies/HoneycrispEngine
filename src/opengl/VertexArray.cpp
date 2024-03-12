#include "VertexArray.h"


HNCRSP_NAMESPACE_START

VertexArray::VertexArray(
    const std::vector<glm::vec3>* vertices,
    const std::vector<GLuint>* indices,
    const std::vector<glm::vec3>* normals,
    const std::vector<glm::vec3>* colors,
    const std::vector<glm::vec2>* uvs
)
{
    // Preliminary checks
    if (vertices == nullptr || indices == nullptr)
    {
        HNCRSP_TERMINATE("Missing vertex positions and indices.");
    }

    if (!m_vertData.empty())
    {
        HNCRSP_LOG_WARN("Mesh already constructed.");
        return;
    }

    if (vertices->empty())
    {
        HNCRSP_TERMINATE("No data to construct mesh.");
    }
    if (indices->empty())
    {
        HNCRSP_TERMINATE("No indices to draw mesh.");
    }
    if (normals)
    {
        if (normals->size() != vertices->size() && normals->size() != 0)
        {
            HNCRSP_TERMINATE("Normals data not uniform with vertex positions.");
        }
    }
    if (colors)
    {
        if (colors->size() != vertices->size() && colors->size() != 0)
        {
            HNCRSP_TERMINATE("Color data not uniform with vertex positions.");
        }
    }
    if (uvs)
    {
        if (uvs->size() != vertices->size() && uvs->size() != 0)
        {
            HNCRSP_TERMINATE("UV data not uniform with vertex positions.");
        }
    }
    
    size_t vertArrayDataSize = vertices->size()*3 + vertices->size()*3;
    if (normals) vertArrayDataSize += normals->size();
    if (uvs) vertArrayDataSize += uvs->size();
    m_vertData.reserve(vertArrayDataSize);

    for (size_t vertIndex = 0; vertIndex < vertices->size(); vertIndex++)
    {
        if (!vertices->empty())
        {
            m_vertData.push_back(vertices->at(vertIndex).x);
            m_vertData.push_back(vertices->at(vertIndex).y);
            m_vertData.push_back(vertices->at(vertIndex).z);
        }

        if (!colors)  // even if color is not available there should be a default color
        {
            m_vertData.push_back(1.0f);
            m_vertData.push_back(1.0f);
            m_vertData.push_back(1.0f);
        }
        else
        {
            m_vertData.push_back(colors->at(vertIndex).x);
            m_vertData.push_back(colors->at(vertIndex).y);
            m_vertData.push_back(colors->at(vertIndex).z);
        }

        if (uvs)
        {
            m_vertData.push_back(uvs->at(vertIndex).x);
            m_vertData.push_back(uvs->at(vertIndex).y);
        }

        if (normals)
        {
            m_vertData.push_back(normals->at(vertIndex).x);
            m_vertData.push_back(normals->at(vertIndex).y);
            m_vertData.push_back(normals->at(vertIndex).z);
        }
    }

    CreateVAO(
        m_vertData.data(),
        m_vertData.size() * sizeof(float),
        indices->data(),
        indices->size() * sizeof(GLuint), 
        GL_STATIC_DRAW
    );


    size_t numAttribElements = 
        3 +
        3 +
        2 * !uvs +
        3 * !normals;
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

    if (uvs)
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

    if (normals)
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

void VertexArray::EnableVertexAttribPosition(bool on) const
{
    // Becareful of these binds
    Bind();
    if (on)
    {
        GLCall(glEnableVertexAttribArray(0));
    }
    else
    {
        GLCall(glDisableVertexAttribArray(0));
    }
}

void VertexArray::EnableVertexAttribColor(bool on) const
{
    Bind();
    if (on)
    {
        GLCall(glEnableVertexAttribArray(1));
    }
    else
    {
        GLCall(glDisableVertexAttribArray(1));
    }
}

void VertexArray::EnableVertexAttribUV(bool on) const
{
    Bind();
    if (on)
    {
        GLCall(glEnableVertexAttribArray(2));
    }
    else
    {
        GLCall(glDisableVertexAttribArray(2));
    }
}

void VertexArray::EnableVertexAttribNormals(bool on) const
{
    Bind();
    if (on)
    {
        GLCall(glEnableVertexAttribArray(3));
    }
    else
    {
        GLCall(glDisableVertexAttribArray(3));
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
    m_VAO_ID = other.m_VAO_ID;
    other.m_VAO_ID = 0;

    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_elementBuffer = std::move(other.m_elementBuffer);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    m_VAO_ID = other.m_VAO_ID;
    other.m_VAO_ID = 0;

    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_elementBuffer = std::move(other.m_elementBuffer);

    return *this;
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_VAO_ID));
}

void VertexArray::CreateVAO(const float* vboData, size_t vboSize, const GLuint* eboData, size_t eboSize, GLenum mode)
{
    GLCall(glGenVertexArrays(1, &m_VAO_ID));
    Bind();
    m_vertexBuffer.CreateVBO(vboData, vboSize, mode);
    m_elementBuffer.CreateEBO(eboData, eboSize, mode);
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_VAO_ID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}

GLuint VertexArray::getID()
{
    return m_VAO_ID;
}

HNCRSP_NAMESPACE_END