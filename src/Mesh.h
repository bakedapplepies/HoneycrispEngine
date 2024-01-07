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
    std::vector< std::shared_ptr<Texture2D> > textures;

private:
    std::vector<glm::vec3> m_eulerAngles;
    std::vector<float> m_scalars;
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

    virtual void onUpdate() {}
    void Draw(std::shared_ptr<Shader> shader) const;
    void Translate(const glm::vec3& vec);
    void Scale(const float& multiplier);
    void Rotate(const float& rX, const float& rY, const float& rZ);
    glm::mat4 GetModelMatrix(const Transform& position) const;

    void ConstructMesh();
    std::weak_ptr<VertexArray> GetVAO();
};