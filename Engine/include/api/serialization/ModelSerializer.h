#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/serialization/Serializer.h"
#include "api/serialization/schemas/model_generated.h"
#include "api/components/DrawData.h"

HNCRSP_NAMESPACE_START

class ModelSerializer : public Serializer<Serialized::Model>
{
private:
    flatbuffers::FlatBufferBuilder m_builder;
    std::unique_ptr<Serialized::ModelBuilder> m_model_builder;

    uint8_t m_vertex_attrib_bits;
    flatbuffers::Offset< flatbuffers::Vector<float> > m_vertex_data;
    flatbuffers::Offset< flatbuffers::Vector<GLuint> > m_indices;
    flatbuffers::Offset< flatbuffers::Vector<const Serialized::MeshMetaData*> > m_meshes_meta_data;
    std::vector< flatbuffers::Offset<Serialized::Material> > m_materials;

    std::unique_ptr<Serialized::MaterialBuilder> m_material_builder;

    // need this to survive the duration of loading deserialized model into VAO
    std::unique_ptr<char[]> m_serialized_data;

public:
    ModelSerializer();
    ~ModelSerializer() = default;

    void AddModel(
        uint8_t vertex_attrib_bits,
        const float* vertex_data,
        size_t vertex_data_len,
        const GLuint* indices_data,
        size_t indices_data_len,
        const MeshMetaData* mesh_meta_data,
        size_t mesh_meta_data_len
    );
    void AddMaterial(
        const FileSystem::Path& albedo_path,
        const FileSystem::Path& roughness_path,
        const FileSystem::Path& ao_path,
        const FileSystem::Path& normal_path,
        const FileSystem::Path& specular_path
    );
    void Serialize(const FileSystem::Path& path_to_model);
    const Serialized::Model* GetDeserializedObject(const FileSystem::Path& path_to_model);

private:
    flatbuffers::Offset<Serialized::Model> _FinishModel(
        uint64_t last_write_time
    );
};

HNCRSP_NAMESPACE_END