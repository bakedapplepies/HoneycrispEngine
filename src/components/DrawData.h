#pragma once

#include "src/pch/pch.h"
#include "src/opengl/VertexArray.h"
#include "src/managers/Material.h"
#include "src/serialize/schemas/model_generated.h"


HNCRSP_NAMESPACE_START

struct MeshMetaData  // TODO: should this be put here
{
    GLuint mesh_vertex_count;
    GLuint indices_buffer_count;

    operator Serialized::MeshMetaData() const
    {
        return Serialized::MeshMetaData(mesh_vertex_count, indices_buffer_count);
    }
};

struct DrawData
{
    GLuint VAO_id;
    std::shared_ptr<Material> material = nullptr;
    std::vector<MeshMetaData> meta_data;
};

HNCRSP_NAMESPACE_END