#pragma once

#include "src/pch/pch.h"

#include "VertexBuffer.h"
#include "ElementBuffer.h"


HNCRSP_NAMESPACE_START

class VertexArray
{
private:
    GLuint m_VAO_ID;
    VertexBuffer m_vertexBuffer;
    ElementBuffer m_elementBuffer;

    // mesh data
    std::vector<float> m_vertData;

public:
    VertexArray(
        const std::vector<glm::vec3>* vertices,
        const std::vector<GLuint>* indices,
        const std::vector<glm::vec3>* normals,
        const std::vector<glm::vec3>* colors,
        const std::vector<glm::vec2>* uvs
    );
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray& operator=(VertexArray&& other) noexcept;
    ~VertexArray();

    void CreateVAO(const float* vboData, size_t vboSize, const GLuint* eboData, size_t eboSize, GLenum mode);
    void Bind() const;
    void Unbind() const;

    void SubData();  // TODO

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;

    GLuint getID();
};

HNCRSP_NAMESPACE_END