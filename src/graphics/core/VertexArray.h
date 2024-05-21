#pragma once

#include "src/pch/pch.h"

#include "VertexBuffer.h"
#include "ElementBuffer.h"


HNCRSP_NAMESPACE_START

class VertexArray
{
private:
    GLuint m_VAO_ID = 0;
    VertexBuffer m_vertexBuffer;
    ElementBuffer m_elementBuffer;

    // mesh data
    std::vector<float> m_vertexData;
    std::vector<GLuint> m_indices;
    
    void CreateVAO(const float* vboData, size_t vboSize, const GLuint* eboData, size_t eboSize, GLenum mode);

public:
    VertexArray() = default;
    VertexArray(
        const std::vector<glm::vec3>* vertices,
        const std::vector<GLuint>* indices,
        const std::vector<glm::vec3>* normals,
        const std::vector<glm::vec3>* colors,
        const std::vector<glm::vec2>* uvs
    );
    VertexArray(
        unsigned short vertex_attrib_bits,
        const std::vector<float>& vertex_data,
        const std::vector<GLuint>& indices_data
    );
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray& operator=(VertexArray&& other) noexcept;
    ~VertexArray();

    void Bind() const;
    void Unbind() const;
    GLuint getID() const;
    const float* getData() const;
    size_t getDataLen() const;

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;
};

HNCRSP_NAMESPACE_END