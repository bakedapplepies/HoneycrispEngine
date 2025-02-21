#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/VertexArray.h"
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

    void virt_AddDrawDataToRenderer(ECS::EntityUID entityUID) const override final;
};

HNCRSP_NAMESPACE_END