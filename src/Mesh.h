#pragma once

#include "pch/pch.h"

#include "Renderable.h"
#include "core/VertexArray.h"
#include "core/Texture.h"


class Mesh : public Renderable
{
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<GLuint> indices;
    std::vector< std::shared_ptr<Texture> > textures;

private:
    glm::mat4 m_modelMatrix;
    std::vector<float> m_vertData;
    std::shared_ptr<VertexArray> m_VAO;

public:
    Mesh();
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    ~Mesh() = default;

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;

    virtual void OnUpdate() {}
    void Draw(std::shared_ptr<Shader> shader) const;
    void Translate(const glm::vec3& vec);
    void Scale(const float& multiplier);
    void Rotate(const float& rX, const float& rY, const float& rZ);
    void addPosition(const glm::vec3& position);
    glm::mat4 GetModelMatrix(const glm::vec3& position) const;

    void ConstructMesh();
    std::weak_ptr<VertexArray> GetVAO();
};