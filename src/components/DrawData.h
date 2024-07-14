#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/graphics/core/VertexArray.h"
#include "src/graphics/Material.h"
#include "src/serialize/schemas/model_generated.h"


HNCRSP_NAMESPACE_START

struct MeshMetaData;

// Per-scene data
struct DrawData
{
    GLuint VAO_id;
    std::vector< std::shared_ptr<Material> > materials;  // TODO: Maybe remove shaders out of materials
    std::vector<MeshMetaData> meta_data;
};

struct MeshMetaData
{
    GLuint mesh_vertex_count;
    GLuint indices_buffer_count;
    uint32_t material_index;

    operator Serialized::MeshMetaData() const
    {
        return Serialized::MeshMetaData(mesh_vertex_count, indices_buffer_count, material_index);
    }
};

HNCRSP_NAMESPACE_END