#pragma once

#include "src/pch/pch.h"
#include "src/ecs/ECSManager.h"
#include "src/Renderable.h"
#include "src/opengl/VertexArray.h"


HNCRSP_NAMESPACE_START

class Cube : public Renderable
{
private:
    GLuint m_numVertices;

public:
    Cube();
    Cube(const Cube& other) = delete;
    Cube& operator=(const Cube& other) = delete;
    Cube(Cube&& other) noexcept;
    Cube& operator=(Cube&& other) noexcept;
    ~Cube() = default;
    
    void virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material = nullptr) override final;

private:
    void InitializeAttributeData();

private:
    std::unique_ptr<VertexArray> m_VAO;
};

HNCRSP_NAMESPACE_END