#pragma once

#include "pch/pch.h"

#include "VertexBuffer.h"
#include "ElementBuffer.h"


class VertexArray
{
private:
    GLuint m_VAO_ID;
    VertexBuffer m_vertexBuffer;
    ElementBuffer m_elementBuffer;

public:
    VertexArray() = default;
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray& operator=(VertexArray&& other) noexcept;
    ~VertexArray();

    void CreateVAO(float* vboData, size_t vboSize, GLuint* eboData, size_t eboSize, GLenum mode);
    void Bind() const;
    void Unbind() const;

    VertexBuffer* getVBO();
    ElementBuffer* getEBO();

    GLuint getID();
};