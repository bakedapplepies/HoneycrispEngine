#pragma once

#include "pch/pch.h"

#include "Object.h"
#include "VertexArray.h"


class Mesh : public Object
{
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<unsigned int> indices;

private:
    std::vector<float> vertData;
    VertexArray m_VAO;

public:
    Mesh() = default;
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept = delete;
    Mesh operator=(const Mesh& other) = delete;
    Mesh operator=(Mesh&& other) noexcept = delete;
    ~Mesh() = default;

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;

    void Draw(const Shader& shader);
    void Translate(const glm::vec3& vec);
    void Scale(const float& multiplier);
    void Rotate(const float& rX, const float& rY, const float& rZ);
    glm::mat4 GetModelMatrix(const glm::vec3& position) const;

    void ConstructMesh();
    void Bind();
    VertexArray& GetVAO();
};