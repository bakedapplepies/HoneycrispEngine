#pragma once

#include "pch/pch.h"


class ElementBuffer
{
private:
    GLuint m_EBO_ID;

public:
    ElementBuffer() = default;
    ElementBuffer(const ElementBuffer&) = delete;
    ElementBuffer(ElementBuffer&& other) noexcept;
    ElementBuffer& operator=(const ElementBuffer&) = delete;
    ElementBuffer& operator=(ElementBuffer&&) noexcept;
    ~ElementBuffer();
    
    void CreateEBO(unsigned int* data, unsigned int& dataSize, GLenum mode);
    void Bind() const;
    void Unbind() const;
};