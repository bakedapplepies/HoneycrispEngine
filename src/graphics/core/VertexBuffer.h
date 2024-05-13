#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

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
    
    void CreateVBO(const float* data, size_t dataSize);
    void Bind() const;
    void Unbind() const;
};

HNCRSP_NAMESPACE_END