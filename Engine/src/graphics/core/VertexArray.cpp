#include "api/graphics/core/VertexArray.h"

HNCRSP_NAMESPACE_START

VertexArray::VertexArray(
    uint16_t vertex_attrib_bits,
    const std::vector<Vertex>* vertices,
    const std::vector<GLuint>* indices
) {
    std::vector<float> vertexData;
    std::vector<GLuint> indexData;

    // Checks if enough data is given
    if (vertices == nullptr || indices == nullptr)
    {
        HNCRSP_TERMINATE("Missing vertices and indices.");
    }

    if (m_VAO_ID != 0)
    {
        HNCRSP_WARN("Mesh already constructed.");
        return;
    }

    m_vertexAttribBits = vertex_attrib_bits;

    // Check if vectors contain anything
    if (vertices->empty())
    {
        HNCRSP_TERMINATE("No data to construct mesh.");
    }
    if (indices->empty())
    {
        HNCRSP_TERMINATE("No indices to draw mesh.");
    }
    if (!(m_vertexAttribBits & VERTEX_ATTRIB_POSITION_BIT))
    {
        HNCRSP_TERMINATE("Vertex position is mandatory.");
    }

    bool hasColorBit = static_cast<bool>(m_vertexAttribBits & VERTEX_ATTRIB_COLOR_BIT);
    bool hasNormalBit = static_cast<bool>(m_vertexAttribBits & VERTEX_ATTRIB_NORMAL_BIT);
    bool hasUVBit = static_cast<bool>(m_vertexAttribBits & VERTEX_ATTRIB_UV_BIT);
    bool hasTangentBit = static_cast<bool>(m_vertexAttribBits & VERTEX_ATTRIB_TANGENT_BIT);

    indexData = std::vector<GLuint>(indices->begin(), indices->end());
    
    size_t vertArrayDataSize = vertices->size() * 3;
    if (hasColorBit) vertArrayDataSize += vertices->size() * 3;
    if (hasNormalBit) vertArrayDataSize += vertices->size() * 3;
    if (hasUVBit) vertArrayDataSize += vertices->size() * 2;
    if (hasTangentBit) vertArrayDataSize += vertices->size() * 3;
    vertexData.reserve(vertArrayDataSize);
    
    // TODO: Somehow fill this asynchronously
    // perhaps via the GPU?

    // HNCRSP_INFO(vertices->size());
// #define MAX_VERTICES_PER_PROCESS 3000
//     unsigned int num_processes =
//         static_cast<unsigned int>(1.0 * vertArrayDataSize / MAX_VERTICES_PER_PROCESS + 0.5);
//     if (num_processes == 0) num_processes = 1;

    const std::vector<Vertex>& verticesRef = *vertices;

    for (size_t vertIndex = 0; vertIndex < vertices->size(); vertIndex++)
    {
        vertexData.push_back(verticesRef[vertIndex].position.x);
        vertexData.push_back(verticesRef[vertIndex].position.y);
        vertexData.push_back(verticesRef[vertIndex].position.z);

        if (hasColorBit)
        {
            vertexData.push_back(verticesRef[vertIndex].color.x);
            vertexData.push_back(verticesRef[vertIndex].color.y);
            vertexData.push_back(verticesRef[vertIndex].color.z);
        }
        if (hasUVBit)
        {
            vertexData.push_back(verticesRef[vertIndex].uv.x);
            vertexData.push_back(verticesRef[vertIndex].uv.y);
        }
        if (hasNormalBit)
        {
            vertexData.push_back(verticesRef[vertIndex].normal.x);
            vertexData.push_back(verticesRef[vertIndex].normal.y);
            vertexData.push_back(verticesRef[vertIndex].normal.z);
        }
        if (hasTangentBit)
        {
            vertexData.push_back(verticesRef[vertIndex].tangent.x);
            vertexData.push_back(verticesRef[vertIndex].tangent.y);
            vertexData.push_back(verticesRef[vertIndex].tangent.z);
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
        3 * static_cast<uint64_t>(hasColorBit) +
        2 * static_cast<uint64_t>(hasUVBit) +
        3 * static_cast<uint64_t>(hasNormalBit) +
        3 * static_cast<uint64_t>(hasTangentBit);
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
    if (hasColorBit)
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

    if (hasUVBit)
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

    if (hasNormalBit)
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
    
    if (hasTangentBit)
    {
        // Tangents XYZ (Normalized vectors)
        glVertexAttribPointer(
            VERTEX_ATTRIB_TANGENT_INDEX,
            3,
            GL_FLOAT, 
            GL_FALSE,
            numAttribElements * sizeof(float),
            reinterpret_cast<void*>(currentOffset * sizeof(float))
        );
        EnableVertexAttribTangents(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribTangents(false); }
}

VertexArray::VertexArray(
    uint16_t vertex_attrib_bits,
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

    if (!(m_vertexAttribBits & VERTEX_ATTRIB_POSITION_BIT))
    {
        HNCRSP_TERMINATE("Vertex position is mandatory.");
    }

    bool hasColorBit = static_cast<bool>(m_vertexAttribBits & VERTEX_ATTRIB_COLOR_BIT);
    bool hasNormalBit = static_cast<bool>(m_vertexAttribBits & VERTEX_ATTRIB_NORMAL_BIT);
    bool hasUVBit = static_cast<bool>(m_vertexAttribBits & VERTEX_ATTRIB_UV_BIT);
    bool hasTangentBit = static_cast<bool>(m_vertexAttribBits & VERTEX_ATTRIB_TANGENT_BIT);

    size_t numAttribElements = 
        3 +
        3 * static_cast<uint64_t>(hasColorBit) +
        2 * static_cast<uint64_t>(hasUVBit) +
        3 * static_cast<uint64_t>(hasNormalBit) +
        3 * static_cast<uint64_t>(hasTangentBit);
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
    if (hasColorBit)
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

    if (hasUVBit)
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

    if (hasNormalBit)
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
    
    if (hasTangentBit)
    {
        // Tangents XYZ (Normalized vectors)
        glVertexAttribPointer(
            VERTEX_ATTRIB_TANGENT_INDEX,
            3,
            GL_FLOAT, 
            GL_FALSE,
            numAttribElements * sizeof(float),
            reinterpret_cast<void*>(currentOffset * sizeof(float))
        );
        EnableVertexAttribTangents(true);
        currentOffset += 3;
    }
    else { EnableVertexAttribTangents(false); }
}

void VertexArray::EnableVertexAttribPosition(bool on) const
{
    Bind();
    if (on)
        glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION_INDEX);
    else
        glDisableVertexAttribArray(VERTEX_ATTRIB_POSITION_INDEX);
}

void VertexArray::EnableVertexAttribColor(bool on) const
{
    Bind();
    if (on)
        glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR_INDEX);
    else
        glDisableVertexAttribArray(VERTEX_ATTRIB_COLOR_INDEX);
}

void VertexArray::EnableVertexAttribUV(bool on) const
{
    Bind();
    if (on)
        glEnableVertexAttribArray(VERTEX_ATTRIB_UV_INDEX);
    else
        glDisableVertexAttribArray(VERTEX_ATTRIB_UV_INDEX);
}

void VertexArray::EnableVertexAttribNormals(bool on) const
{
    Bind();
    if (on)
        glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL_INDEX);
    else
        glDisableVertexAttribArray(VERTEX_ATTRIB_NORMAL_INDEX);
}

void VertexArray::EnableVertexAttribTangents(bool on) const
{
    Bind();
    if (on)
        glEnableVertexAttribArray(VERTEX_ATTRIB_TANGENT_INDEX);
    else
        glDisableVertexAttribArray(VERTEX_ATTRIB_TANGENT_INDEX);
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

void VertexArray::PrintVertexFormat() const
{
    uint32_t vertexAttribCount = GetVertexAttribCount();
    HNCRSP_INFO("Vertex Format of VAO ID: {}", m_VAO_ID);
    uint32_t totalBytes = 0;
    if ((m_vertexAttribBits & VERTEX_ATTRIB_POSITION_INDEX) == VERTEX_ATTRIB_POSITION_INDEX)
    {
        HNCRSP_INFO("\tPosition: vec3f ({} bytes, offset {} bytes)", sizeof(float) * 3, totalBytes);
        totalBytes += sizeof(float) * 3;
    }
    if ((m_vertexAttribBits & VERTEX_ATTRIB_COLOR_INDEX) == VERTEX_ATTRIB_COLOR_INDEX)
    {
        HNCRSP_INFO("\tColor: vec3f ({} bytes, offset {} bytes)", sizeof(float) * 3, totalBytes);
        totalBytes += sizeof(float) * 3;
    }
    if ((m_vertexAttribBits & VERTEX_ATTRIB_NORMAL_INDEX) == VERTEX_ATTRIB_NORMAL_INDEX)
    {
        HNCRSP_INFO("\tNormal: vec3f ({} bytes, offset {} bytes)", sizeof(float) * 3, totalBytes);
        totalBytes += sizeof(float) * 3;
    }
    if ((m_vertexAttribBits & VERTEX_ATTRIB_UV_INDEX) == VERTEX_ATTRIB_UV_INDEX)
    {
        HNCRSP_INFO("\tUV: vec2f ({} bytes, offset {} bytes)", sizeof(float) * 2, totalBytes);
        totalBytes += sizeof(float) * 3;
    }
    HNCRSP_INFO("\tTotal bytes/vertices: {}", totalBytes);
}

HNCRSP_NAMESPACE_END