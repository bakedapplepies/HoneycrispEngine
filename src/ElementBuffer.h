#pragma once

#include <glad/glad.h>


class ElementBuffer
{
private:
    GLuint m_EBO_ID;

public:
    void CreateEBO(unsigned int* data, unsigned int& dataSize, GLenum mode);
    void Bind() const;
    void Unbind() const;
    void Delete() const;

};