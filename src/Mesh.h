#pragma once

#include "src/pch/pch.h"
#include "src/components/Transform.h"
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
    // glm::vec3 m_relativeOrigin = glm::vec3(0.0f);

public:
    Mesh(
        std::vector<glm::vec3>* vertices,
        std::vector<GLuint>* indices,
        std::vector<glm::vec3>* normals,
        std::vector<glm::vec3>* colors,
        std::vector<glm::vec2>* uvs
    );
    Mesh(
        unsigned char vertex_attrib_bits,
        const float* vertex_data,
        size_t vertex_data_len,
        const GLuint* indices_data,
        size_t indices_data_len
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
    const VertexArray* GetVAO() const;

    void virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material = nullptr) override final;

// public:
//     inline glm::vec3 getRelativeOrigin() const
//     {
//         return m_relativeOrigin;
//     }

//     inline void setRelativeOrigin(const glm::vec3& relOrigin)
//     {
//         m_relativeOrigin = relOrigin;
//     }
};

HNCRSP_NAMESPACE_END