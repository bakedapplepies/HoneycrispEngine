#pragma once

#include "api/pch/hncrsp_pch.h"

#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "api/serialization/schemas/model_generated.h"

HNCRSP_NAMESPACE_START

struct Vertex
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 uv = glm::vec2(0.0f, 0.0f);
    glm::vec3 tangent = glm::vec3(0.0f, 0.0f, 0.0f);
};

class VertexArray
{
private:
    GLuint m_VAO_ID = 0;
    VertexBuffer m_vertexBuffer;
    ElementBuffer m_elementBuffer;
    uint16_t m_vertexAttribBits = 0;

    // mesh data
    uint32_t m_verticesCount;
    GLuint m_indicesCount;
    
private:
    void _CreateVAO(const float* vboData, size_t vboSize, const GLuint* eboData, size_t eboSize, GLenum mode);

public:
    VertexArray() = default;
    VertexArray(
        uint16_t vertex_attrib_bits,
        const std::vector<Vertex>* vertices,
        const std::vector<GLuint>* indices
    );
    VertexArray(
        uint16_t vertex_attrib_bits,
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
    GLuint GetID() const;
    size_t GetIndicesLen() const;
    uint32_t GetVertexAttribCount() const;
    uint32_t GetVertexCount() const;
    void PrintVertexFormat() const;

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;
    void EnableVertexAttribTangents(bool on) const;
};

HNCRSP_NAMESPACE_END