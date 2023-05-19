#pragma once

#include "pch/pch.h"


class VertexBuffer
{
private:
    GLuint m_VBO_ID;

public:
    VertexBuffer() = default;
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;
    ~VertexBuffer();
    
    void CreateVBO(float* data, unsigned int& dataSize, GLenum mode);
    void Bind() const;
    void Unbind() const;
};