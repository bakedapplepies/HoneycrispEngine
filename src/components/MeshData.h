#pragma once

#include "src/pch/pch.h"
#include "src/opengl/VertexArray.h"
#include "src/core/Material.h"


HNCRSP_NAMESPACE_START

struct MeshData
{
    GLuint VAO_id;
    uint32_t num_vertices;
    std::shared_ptr<Material> material;
    // if material is shared between multiple meshes, it shouldn't be too bad
    // especially in this amateur project lul
};

HNCRSP_NAMESPACE_END