#pragma once

#include "src/pch/pch.h"
#include "src/components/Transform.h"
#include "opengl/VertexArray.h"
#include "src/opengl/Texture2D.h"
#include "src/managers/Material.h"
#include "src/Renderable.h"


HNCRSP_NAMESPACE_START

class Mesh : public Renderable
{
private:
    // still has to be here to manange gpu memory
    std::unique_ptr<VertexArray> m_VAO;
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
        const std::vector<float>& vertex_data,
        const std::vector<GLuint>& indices_data
    );
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    virtual ~Mesh() = default;

    void EnableVertexAttribPosition(bool on) const;
    void EnableVertexAttribColor(bool on) const;
    void EnableVertexAttribUV(bool on) const;
    void EnableVertexAttribNormals(bool on) const;
    // const VertexArray* GetVAO() const;

    void virt_AddDrawDataToRenderer(EntityUID entityUID) override final;
};

HNCRSP_NAMESPACE_END