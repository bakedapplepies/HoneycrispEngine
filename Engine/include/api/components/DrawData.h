#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/VertexArray.h"
#include "api/graphics/Material.h"
#include "api/serialization/schemas/model_generated.h"

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
    GLuint mesh_vertex_offset;
    GLuint indices_buffer_count;
    uint32_t material_index;

    operator Serialized::MeshMetaData() const
    {
        return Serialized::MeshMetaData(mesh_vertex_offset, indices_buffer_count, material_index);
    }
};

HNCRSP_NAMESPACE_END