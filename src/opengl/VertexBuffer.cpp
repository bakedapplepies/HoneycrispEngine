#include "VertexBuffer.h"


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
    GLCall(glDeleteBuffers(1, &m_VBO_ID));
}

void VertexBuffer::CreateVBO(const float* data, size_t dataSize, GLenum mode)
{
    GLCall(glGenBuffers(1, &m_VBO_ID));
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, dataSize, data, mode));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

HNCRSP_NAMESPACE_END