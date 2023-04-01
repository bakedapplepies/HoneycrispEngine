#pragma once

#include <glad/glad.h>

#include "VertexBuffer.h"
#include "ElementBuffer.h"


class VertexArray
{
private:
    GLuint m_VAO_ID;
    VertexBuffer m_vertexBuffer;
    ElementBuffer m_elementBuffer;

public:
    void CreateVAO(float* vboData, unsigned int vboSize, unsigned int* eboData, unsigned int eboSize, GLenum mode);
    void Bind() const;
    void Unbind() const;
    void Delete() const;

    VertexBuffer* getVBO();
    ElementBuffer* getEBO();

    GLuint getID();
};