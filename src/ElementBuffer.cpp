#include "pch/pch.h"

#include "Debug.h"
#include "ElementBuffer.h"


void ElementBuffer::CreateEBO(unsigned int* data, unsigned int& dataSize, GLenum mode)
{
    GLCall(glGenBuffers(1, &m_EBO_ID));
    Bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, mode));
}

void ElementBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_ID));
}

void ElementBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void ElementBuffer::Delete() const
{
    GLCall(glDeleteBuffers(1, &m_EBO_ID));
}