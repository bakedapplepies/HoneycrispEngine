#pragma once

#include "pch/pch.h"

#include "Object.h"
#include "VertexArray.h"


class Mesh : public Object
{
public:
    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<float> normals;
    std::vector<float> uv;
    std::vector<unsigned int> indices;

    Shader shader;

private:
    std::vector<float> vertData;
    VertexArray m_VAO;

public:
    Mesh() = default;
    ~Mesh() = default;

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;

    void Draw();
    glm::mat4 GetModelMatrix(const glm::vec3& position) const;
    inline Shader& GetShader() { return shader; };

    void ConstructMesh();
    void Bind();
    VertexArray& GetVAO();
};