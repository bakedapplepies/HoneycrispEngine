#pragma once

#include "src/pch/pch.h"

#include "VertexBuffer.h"
#include "ElementBuffer.h"


HNCRSP_NAMESPACE_START

#define HNCRSP_VERTEX_ATTRIB_POSITION_INDEX 0
#define HNCRSP_VERTEX_ATTRIB_COLOR_INDEX    1
#define HNCRSP_VERTEX_ATTRIB_UV_INDEX       2
#define HNCRSP_VERTEX_ATTRIB_NORMAL_INDEX   3

const unsigned short VERTEX_ATTRIB_POSITION_BIT = 1 << HNCRSP_VERTEX_ATTRIB_POSITION_INDEX;
const unsigned short VERTEX_ATTRIB_COLOR_BIT = 1 << HNCRSP_VERTEX_ATTRIB_COLOR_INDEX;
const unsigned short VERTEX_ATTRIB_UV_BIT = 1 << HNCRSP_VERTEX_ATTRIB_UV_INDEX;
const unsigned short VERTEX_ATTRIB_NORMAL_BIT = 1 << HNCRSP_VERTEX_ATTRIB_NORMAL_INDEX;

class VertexArray
{
private:
    GLuint m_VAO_ID = 0;
    VertexBuffer m_vertexBuffer;
    ElementBuffer m_elementBuffer;

    // mesh data
    std::vector<float> m_vertexData;

public:
    VertexArray(
        const std::vector<glm::vec3>* vertices,
        const std::vector<GLuint>* indices,
        const std::vector<glm::vec3>* normals,
        const std::vector<glm::vec3>* colors,
        const std::vector<glm::vec2>* uvs
    );
    VertexArray(
        unsigned short vertex_attrib_bits,
        const float* vertex_data,
        size_t vertex_data_len,
        const GLuint* indices_data,
        size_t indices_data_len
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
    const float* getData() const;
    size_t getDataLen() const;
};

HNCRSP_NAMESPACE_END