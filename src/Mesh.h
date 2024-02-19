#pragma once

#include "src/pch/pch.h"
#include "src/types/Transform.h"
#include "renderer/VertexArray.h"
#include "src/renderer/Texture2D.h"
#include "src/core/Material.h"
#include "src/Object.h"


HNCRSP_NAMESPACE_START

class Mesh : public Object
{
public:
    // scene data
    std::vector<Transform> transforms;

    // mesh data
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<GLuint> indices;

    // mesh props
    std::unique_ptr<Material> material;

private:
    // mesh data
    std::vector<float> m_vertData;
    VertexArray m_VAO;
    glm::mat4 m_modelMatrix;

    // scene data
    glm::vec3 m_relativeOrigin = glm::vec3(0.0f);
    std::vector<glm::vec3> m_eulerAngles;


public:
    Mesh() = default;
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    virtual ~Mesh() = default;

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;

    virtual void OnUpdate() override;
    virtual void Draw(Shader* shader) const;
    void Translate(const glm::vec3& vec);
    void Scale(const float& multiplier);
    void Rotate(const float& rX, const float& rY, const float& rZ);
    glm::mat4 GetModelMatrix(const Transform& position) const;

    void ConstructMesh();

public:

    inline glm::vec3 getRelativeOrigin() const
    {
        return m_relativeOrigin;
    }

    inline void setRelativeOrigin(const glm::vec3& relOrigin)
    {
        m_relativeOrigin = relOrigin;
    }

    virtual inline void addTransform(const Transform& transform)
    {
        transforms.push_back(transform);
    }

    virtual void Draw(Shader* shader);
};

HNCRSP_NAMESPACE_END