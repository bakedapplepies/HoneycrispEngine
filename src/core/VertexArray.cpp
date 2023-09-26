#include "../Debug.h"
#include "VertexArray.h"


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

void VertexArray::CreateVAO(float* vboData, size_t vboSize, GLuint* eboData, size_t eboSize, GLenum mode)
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

VertexBuffer* VertexArray::getVBO()
{
    return &m_vertexBuffer;
}

ElementBuffer* VertexArray::getEBO()
{
    return &m_elementBuffer;
}