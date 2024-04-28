#pragma once

#include "src/pch/pch.h"
#include "src/opengl/VertexArray.h"
#include "src/managers/Material.h"
#include "src/serialize/schemas/model_generated.h"


HNCRSP_NAMESPACE_START

struct MeshMetaData  // TODO: should this be put here
{
    GLuint index_offset;
    GLuint vertex_count;

    operator Serialized::MeshMetaData() const
    {
        return Serialized::MeshMetaData(index_offset, vertex_count);
    }
};

struct DrawData
{
    GLuint VAO_id;
    std::shared_ptr<Material> material = nullptr;
    std::vector<MeshMetaData> meta_data;
};

HNCRSP_NAMESPACE_END