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

    std::vector<float> vertData;
    std::vector<unsigned int> indices;

private:
    VertexArray m_VAO;

public:
    Mesh() = default;
    Mesh(
        const std::vector<float>& vertices,
        const std::vector<float>& colors,
        const std::vector<float>& normals,
        const std::vector<float>& uv,
        const std::vector<unsigned int>& indices
    );
    ~Mesh();

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;

    void Draw() const override;
    void AddPosition(const glm::vec3& position) override;
    std::vector<glm::vec3>& GetPositions() override;
    glm::mat4 GetModelMatrix(const glm::vec3& position) const override;
    Shader& GetShader() override;

    void ConstructMesh();
    void Bind();
    VertexArray& GetVAO();
};