#pragma once

#include "pch/pch.h"

#include "Object.h"
#include "Mesh.h"


class Cube : public Object
{
private:
    Mesh& cubeMesh;
    Shader m_shader;

    glm::vec3 m_position;

public:
    Cube(const glm::vec3& position);
    ~Cube() override;

    void Draw() const override;
    void BuildMeshData() override;
    glm::vec3& GetPosition() override;
    glm::mat4 GetModelMatrix() const override;
    Shader& GetShader() override;
};