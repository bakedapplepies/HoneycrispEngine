#include "api/graphics/core/VertexArray.h"

HNCRSP_NAMESPACE_START

VertexArray::VertexArray(
    const std::vector<glm::vec3>* vertices,
    const std::vector<GLuint>* indices,
    const std::vector<glm::vec3>* normals,
    const std::vector<glm::vec3>* colors,
    const std::vector<glm::vec2>* uvs
) {
    std::vector<float> vertexData;
    std::vector<GLuint> indexData;

    // Preliminary checks
    if (vertices == nullptr || indices == nullptr)
    {
        HNCRSP_TERMINATE("Missing vertex positions and indices.");
    }

    if (m_VAO_ID != 0)
    {
        HNCRSP_WARN("Mesh already constructed.");
        return;
    }

    // Check if vectors contain anything
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
        if (normals->size() != vertices->size())
        {
            HNCRSP_TERMINATE("Normals data not uniform with vertex positions.");
        }
    }
    if (colors)
    {
        if (colors->size() != vertices->size())
        {
            HNCRSP_TERMINATE("Color data not uniform with vertex positions.");
        }
    }
    if (uvs)
    {
        if (uvs->size() != vertices->size())
        {
            HNCRSP_TERMINATE("UV data not uniform with vertex positions.");
        }
    }

    indexData = std::vector<GLuint>(indices->begin(), indices->end());
    
    size_t vertArrayDataSize = vertices->size() * 3;
    if (colors) vertArrayDataSize += colors->size() * 3;
    if (normals) vertArrayDataSize += normals->size() * 3;
    if (uvs) vertArrayDataSize += uvs->size() * 2;
    vertexData.reserve(vertArrayDataSize);
    
    // TODO: Somehow fill this asynchronously
    // perhaps via the GPU?

    // HNCRSP_INFO(vertices->size());
// #define MAX_VERTICES_PER_PROCESS 3000
//     unsigned int num_processes =
//         static_cast<unsigned int>(1.0 * vertArrayDataSize / MAX_VERTICES_PER_PROCESS + 0.5);
//     if (num_processes == 0) num_processes = 1;

    const std::vector<glm::vec3>& verticesRef = *vertices;
    const std::vector<glm::vec3>& colorsRef = *colors;
    const std::vector<glm::vec2>& uvsRef = *uvs;
    const std::vector<glm::vec3>& normalsRef = *normals;

    for (size_t vertIndex = 0; vertIndex < vertices->size(); vertIndex++)
    {
        vertexData.push_back(verticesRef[vertIndex].x);
        vertexData.push_back(verticesRef[vertIndex].y);
        vertexData.push_back(verticesRef[vertIndex].z);
        m_vertexAttribBits |= VERTEX_ATTRIB_POSITION_BIT;

        if (colors)
        {
            vertexData.push_back(colorsRef[vertIndex].x);
            vertexData.push_back(colorsRef[vertIndex].y);
            vertexData.push_back(colorsRef[vertIndex].z);
            m_vertexAttribBits |= VERTEX_ATTRIB_COLOR_BIT;
        }

        if (uvs)
        {
            vertexData.push_back(uvsRef[vertIndex].x);
            vertexData.push_back(uvsRef[vertIndex].y);
            m_vertexAttribBits |= VERTEX_ATTRIB_UV_BIT;
        }

        if (normals)
        {
            vertexData.push_back(normalsRef[vertIndex].x);
            vertexData.push_back(normalsRef[vertIndex].y);
            vertexData.push_back(normalsRef[vertIndex].z);
            m_vertexAttribBits |= VERTEX_ATTRIB_NORMAL_BIT;
        }
    }

    _CreateVAO(
        vertexData.data(),
        vertexData.size() * sizeof(float),
        indices->data(),
        indices->size() * sizeof(GLuint), 
        GL_STATIC_DRAW
    );

    size_t numAttribElements = 
        3 +
        3 * static_cast<bool>(colors) +
        2 * static_cast<bool>(uvs) +
        3 * static_cast<bool>(normals);
    unsigned int currentOffset = 0;

    // Postions XYZ
    glVertexAttribPointer(
        VERTEX_ATTRIB_POSITION_INDEX,
        3,
        GL_FLOAT,
        GL_FALSE,
        numAttribElements * sizeof(float),
        reinterpret_cast<void*>(currentOffset * sizeof(float))
    );
    EnableVertexAttribPosition(true);
    currentOffset += 3;

    // Color RGB
    if (colors)
    {
        glVertexAttribPointer(
            VERTEX_ATTRIB_COLOR_INDEX,
            3,
            GL_FLOAT,
            GL_FALSE,
            numAttribElements * sizeof(float),
            reinterpret_cast<void*>(currentOffset * sizeof(float))
        );
        EnableVertexAttribColor(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribColor(false); }

    if (uvs)
    {
        // Texture Coordinates XY
        glVertexAttribPointer(
            VERTEX_ATTRIB_UV_INDEX,
            2,
            GL_FLOAT,
            GL_FALSE,
            numAttribElements * sizeof(float),
            reinterpret_cast<void*>(currentOffset * sizeof(float))
        );
        EnableVertexAttribUV(true);
        currentOffset += 2;
    }
    else { EnableVertexAttribUV(false); }

    if (normals)
    {
        // Normals XYZ (Normalized vectors)
        glVertexAttribPointer(
            VERTEX_ATTRIB_NORMAL_INDEX,
            3,
            GL_FLOAT, 
            GL_FALSE,
            numAttribElements * sizeof(float),
            reinterpret_cast<void*>(currentOffset * sizeof(float))
        );
        EnableVertexAttribNormals(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribNormals(false); }
}

VertexArray::VertexArray(
    uint8_t vertex_attrib_bits,
    const std::vector<float>& vertex_data,
    const std::vector<GLuint>& indices_data
) {
    std::vector<float> vertexData = std::vector<float>(vertex_data.begin(), vertex_data.end());
    std::vector<GLuint> indexData = std::vector<GLuint>(indices_data.begin(), indices_data.end());
    m_vertexAttribBits = vertex_attrib_bits;

    _CreateVAO(
        vertexData.data(),
        vertexData.size() * sizeof(float),
        indexData.data(),
        indexData.size() * sizeof(GLuint),
        GL_STATIC_DRAW
    );

    bool pos = (vertex_attrib_bits & VERTEX_ATTRIB_POSITION_BIT) == VERTEX_ATTRIB_POSITION_BIT;
    bool color = (vertex_attrib_bits & VERTEX_ATTRIB_COLOR_BIT) == VERTEX_ATTRIB_COLOR_BIT;
    bool uv = (vertex_attrib_bits & VERTEX_ATTRIB_UV_BIT) == VERTEX_ATTRIB_UV_BIT;
    bool normal = (vertex_attrib_bits & VERTEX_ATTRIB_NORMAL_BIT) == VERTEX_ATTRIB_NORMAL_BIT;

    size_t numAttribElements = 
        3 +
        3 * static_cast<size_t>(color) +
        2 * static_cast<size_t>(uv) +
        3 * static_cast<size_t>(normal);
    unsigned int currentOffset = 0;

    // Postions XYZ
    glVertexAttribPointer(
        VERTEX_ATTRIB_POSITION_INDEX,
        3,
        GL_FLOAT,
        GL_FALSE,
        numAttribElements * sizeof(float),
        reinterpret_cast<void*>(currentOffset * sizeof(float))
    );
    EnableVertexAttribPosition(true);
    currentOffset += 3;

    // Color RGB
    if (color)
    {
        glVertexAttribPointer(
            VERTEX_ATTRIB_COLOR_INDEX,
            3,
            GL_FLOAT,
            GL_FALSE,
            numAttribElements * sizeof(float),
            reinterpret_cast<void*>(currentOffset * sizeof(float))
        );
        EnableVertexAttribColor(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribColor(false); }

    if (uv)
    {
        // Texture Coordinates XY
        glVertexAttribPointer(
            VERTEX_ATTRIB_UV_INDEX,
            2,
            GL_FLOAT,
            GL_FALSE,
            numAttribElements * sizeof(float),
            reinterpret_cast<void*>(currentOffset * sizeof(float))
        );
        EnableVertexAttribUV(true);
        currentOffset += 2;
    }
    else { EnableVertexAttribUV(false); }

    if (normal)
    {
        // Normals XYZ (Normalized vectors)
        glVertexAttribPointer(
            VERTEX_ATTRIB_NORMAL_INDEX,
            3,
            GL_FLOAT, 
            GL_FALSE,
            numAttribElements * sizeof(float),
            reinterpret_cast<void*>(currentOffset * sizeof(float))
        );
        EnableVertexAttribNormals(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribNormals(false); }
}

void VertexArray::EnableVertexAttribPosition(bool on) const
{
    Bind();
    if (on)
    {
        glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION_INDEX);
    }
    else
    {
        glDisableVertexAttribArray(VERTEX_ATTRIB_POSITION_INDEX);
    }
}

void VertexArray::EnableVertexAttribColor(bool on) const
{
    Bind();
    if (on)
    {
        glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR_INDEX);
    }
    else
    {
        glDisableVertexAttribArray(VERTEX_ATTRIB_COLOR_INDEX);
    }
}

void VertexArray::EnableVertexAttribUV(bool on) const
{
    Bind();
    if (on)
    {
        glEnableVertexAttribArray(VERTEX_ATTRIB_UV_INDEX);
    }
    else
    {
        glDisableVertexAttribArray(VERTEX_ATTRIB_UV_INDEX);
    }
}

void VertexArray::EnableVertexAttribNormals(bool on) const
{
    Bind();
    if (on)
    {
        glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL_INDEX);
    }
    else
    {
        glDisableVertexAttribArray(VERTEX_ATTRIB_NORMAL_INDEX);
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
    m_VAO_ID = other.m_VAO_ID;
    other.m_VAO_ID = 0;
    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_elementBuffer = std::move(other.m_elementBuffer);

    m_verticesCount = other.m_verticesCount;
    m_indicesCount = other.m_indicesCount;
    other.m_verticesCount = 0;
    other.m_indicesCount = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    m_VAO_ID = other.m_VAO_ID;
    other.m_VAO_ID = 0;
    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_elementBuffer = std::move(other.m_elementBuffer);

    m_verticesCount = other.m_verticesCount;
    m_indicesCount = other.m_indicesCount;
    other.m_verticesCount = 0;
    other.m_indicesCount = 0;

    return *this;
}

VertexArray::~VertexArray()
{
    HNCRSP_CHECK_RENDER_CONTEXT();

    glDeleteVertexArrays(1, &m_VAO_ID);
}

void VertexArray::_CreateVAO(const float* vboData, size_t vboSize, const GLuint* eboData, size_t eboSize, GLenum mode)
{
    if (m_VAO_ID != 0)
    {
        HNCRSP_TERMINATE("VAO already created");
    }
    glGenVertexArrays(1, &m_VAO_ID);
    Bind();
    m_vertexBuffer.CreateVBO(vboData, vboSize);
    m_elementBuffer.CreateEBO(eboData, eboSize);

    m_verticesCount = vboSize / GetVertexAttribCount();
    m_indicesCount = eboSize;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_VAO_ID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

GLuint VertexArray::GetID() const
{
    return m_VAO_ID;
}

size_t VertexArray::GetIndicesLen() const
{
    return m_indicesCount;
}

uint32_t VertexArray::GetVertexAttribCount() const
{
    return static_cast<uint32_t>((m_vertexAttribBits & VERTEX_ATTRIB_POSITION_INDEX) == VERTEX_ATTRIB_POSITION_INDEX)
         + static_cast<uint32_t>((m_vertexAttribBits & VERTEX_ATTRIB_COLOR_INDEX) == VERTEX_ATTRIB_COLOR_INDEX)
         + static_cast<uint32_t>((m_vertexAttribBits & VERTEX_ATTRIB_UV_INDEX) == VERTEX_ATTRIB_UV_INDEX)
         + static_cast<uint32_t>((m_vertexAttribBits & VERTEX_ATTRIB_NORMAL_INDEX) == VERTEX_ATTRIB_NORMAL_INDEX);
}

uint32_t VertexArray::GetVertexCount() const
{
    return m_verticesCount;
}

HNCRSP_NAMESPACE_END