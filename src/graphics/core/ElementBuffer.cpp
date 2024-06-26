#include "ElementBuffer.h"


HNCRSP_NAMESPACE_START

ElementBuffer::ElementBuffer(ElementBuffer&& other) noexcept
{
    m_EBO_ID = other.m_EBO_ID;
    other.m_EBO_ID = 0;
}

ElementBuffer& ElementBuffer::operator=(ElementBuffer&& other) noexcept
{
    m_EBO_ID = other.m_EBO_ID;
    other.m_EBO_ID = 0;

    return *this;
}

ElementBuffer::~ElementBuffer()
{
    GLCall(glDeleteBuffers(1, &m_EBO_ID));
}

void ElementBuffer::CreateEBO(const GLuint* data, size_t dataSize)
{
    GLCall(glGenBuffers(1, &m_EBO_ID));
    Bind();
    GLCall(glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_MAP_READ_BIT));
}

void ElementBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_ID));
}

void ElementBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

HNCRSP_NAMESPACE_END