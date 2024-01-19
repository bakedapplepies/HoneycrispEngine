#pragma once

#include "src/pch/pch.h"

#include "Mesh.h"


HNCRSP_NAMESPACE_START

class Cube : public Mesh
{
public:
    Cube();
    Cube(Cube&& other) noexcept = default;
    Cube& operator=(Cube&& other) noexcept = default;
    ~Cube() = default;

private:
    void InitializeAttributeData();

private:
    std::vector<glm::vec3> _verticesPos;
    std::vector<glm::vec3> _colors;
    std::vector<glm::vec3> _normals;
    std::vector<glm::vec2> _uvs;
    std::vector<GLuint> _indices;
};

HNCRSP_NAMESPACE_END