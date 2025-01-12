#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/components/Transform.h"
#include "api/graphics/core/VertexArray.h"
#include "api/graphics/core/Texture2D.h"
#include "api/graphics/Material.h"
#include "api/scene/Renderable.h"


HNCRSP_NAMESPACE_START

class Mesh : public Renderable
{
private:
    VertexArray m_VAO;

public:
    Mesh(
        std::vector<glm::vec3>* vertices,
        std::vector<GLuint>* indices,
        std::vector<glm::vec3>* normals,
        std::vector<glm::vec3>* colors,
        std::vector<glm::vec2>* uvs
    );
    Mesh(
        uint8_t vertex_attrib_bits,
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

    void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID) const override final;
};

HNCRSP_NAMESPACE_END