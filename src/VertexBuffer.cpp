#include <glad/glad.h>

#include "Debug.h"
#include "VertexBuffer.h"


void VertexBuffer::CreateVBO(float* data, unsigned int& dataSize, GLenum mode)
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

void VertexBuffer::Delete() const
{
    GLCall(glDeleteBuffers(1, &m_VBO_ID));
}