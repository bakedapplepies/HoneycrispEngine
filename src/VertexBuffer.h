#pragma once

#include <glad/glad.h>


class VertexBuffer
{
private:
    GLuint m_VBO_ID;

public:
    void CreateVBO(float* data, unsigned int& dataSize, GLenum mode);
    void Bind() const;
    void Unbind() const;
    void Delete() const;

};