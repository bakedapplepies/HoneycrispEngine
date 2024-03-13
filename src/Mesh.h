#pragma once

#include "src/pch/pch.h"
#include "src/components/Transform.h"
#include "src/systems/Renderer.h"
#include "opengl/VertexArray.h"
#include "src/opengl/Texture2D.h"
#include "src/core/Material.h"
#include "src/Renderable.h"


HNCRSP_NAMESPACE_START

class Mesh : public Renderable
{
private:
    // still has to be here to manange gpu memory
    std::shared_ptr<VertexArray> m_VAO;
    uint32_t m_numVertices;

    // scene data
    glm::vec3 m_relativeOrigin = glm::vec3(0.0f);

public:
    Mesh(
        std::vector<glm::vec3>* vertices,
        std::vector<GLuint>* indices,
        std::vector<glm::vec3>* normals,
        std::vector<glm::vec3>* colors,
        std::vector<glm::vec2>* uvs
    );
    Mesh(const Mesh& other);
    Mesh& operator=(const Mesh& other);
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    virtual ~Mesh() = default;

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;

    // TODO: maybe make this part of the Transform class
    void Translate(const glm::vec3& vec);
    void Scale(const float& multiplier);
    void Rotate(const float& rX, const float& rY, const float& rZ);

    void virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material = nullptr) override final;

public:

    inline glm::vec3 getRelativeOrigin() const
    {
        return m_relativeOrigin;
    }

    inline void setRelativeOrigin(const glm::vec3& relOrigin)
    {
        m_relativeOrigin = relOrigin;
    }
};

HNCRSP_NAMESPACE_END