#pragma once

#include "pch/pch.h"

#include "core/Texture.h"
#include "Mesh.h"


class Cube : public Mesh
{
public:
    Cube();
    Cube(Cube&& other) noexcept;
    Cube& operator=(Cube&& other) noexcept;
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