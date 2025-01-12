#include "api/graphics/core/VertexBuffer.h"

HNCRSP_NAMESPACE_START

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
{
    m_VBO_ID = other.m_VBO_ID;
    other.m_VBO_ID = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    m_VBO_ID = other.m_VBO_ID;
    other.m_VBO_ID = 0;

    return *this;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_VBO_ID);
}

void VertexBuffer::CreateVBO(const float* data, size_t dataSize)
{
    glGenBuffers(1, &m_VBO_ID);
    Bind();
    glBufferStorage(GL_ARRAY_BUFFER, dataSize, data, GL_MAP_READ_BIT);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

HNCRSP_NAMESPACE_END