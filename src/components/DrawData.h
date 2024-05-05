#pragma once

#include "src/pch/pch.h"
#include "src/graphics/core/VertexArray.h"
#include "src/graphics/Material.h"
#include "src/serialize/schemas/model_generated.h"


HNCRSP_NAMESPACE_START

struct MeshMetaData;

struct DrawData
{
    GLuint VAO_id;
    std::shared_ptr<Material> material = nullptr;
    std::vector<MeshMetaData> meta_data;
};

struct MeshMetaData
{
    GLuint mesh_vertex_count;
    GLuint indices_buffer_count;
    // std::shared_ptr<Material> material;

    operator Serialized::MeshMetaData() const
    {
        return Serialized::MeshMetaData(mesh_vertex_count, indices_buffer_count);
    }
};

HNCRSP_NAMESPACE_END