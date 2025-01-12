#pragma once

#include "api/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class ElementBuffer
{
private:
    GLuint m_EBO_ID = 0;

public:
    ElementBuffer() = default;
    ElementBuffer(const ElementBuffer&) = delete;
    ElementBuffer(ElementBuffer&& other) noexcept;
    ElementBuffer& operator=(const ElementBuffer&) = delete;
    ElementBuffer& operator=(ElementBuffer&&) noexcept;
    ~ElementBuffer();
    
    void CreateEBO(const GLuint* data, size_t dataSize);
    void Bind() const;
    void Unbind() const;
};

HNCRSP_NAMESPACE_END