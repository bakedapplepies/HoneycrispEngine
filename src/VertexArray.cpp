#include "pch/pch.h"

#include "Debug.h"
#include "VertexArray.h"


void VertexArray::CreateVAO(float* vboData, unsigned int vboSize, unsigned int* eboData, unsigned int eboSize, GLenum mode)
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

void VertexArray::Delete() const
{
    GLCall(glDeleteVertexArrays(1, &m_VAO_ID));
    m_vertexBuffer.Delete();
    m_elementBuffer.Delete();
}

GLuint VertexArray::getID()
{
    return m_VAO_ID;
}

VertexBuffer* VertexArray::getVBO()
{
    return &m_vertexBuffer;
}

ElementBuffer* VertexArray::getEBO()
{
    return &m_elementBuffer;
}