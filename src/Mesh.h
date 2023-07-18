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
    void AddPosition(const glm::vec3& position) override;
    std::vector<glm::vec3>& GetPositions() override;
    glm::mat4 GetModelMatrix(const glm::vec3& position) const;
    Shader& GetShader();

    void ConstructMesh();
    void Bind();
    VertexArray& GetVAO();
};