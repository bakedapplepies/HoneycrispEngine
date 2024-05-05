#pragma once

#include "src/pch/pch.h"
#include "src/serialize/Serializer.h"
#include "src/serialize/schemas/model_generated.h"
#include "src/components/DrawData.h"


HNCRSP_NAMESPACE_START

class ModelSerializer : public Serializer<Serialized::Model>
{
private:
    flatbuffers::FlatBufferBuilder m_builder;
    std::unique_ptr<Serialized::ModelBuilder> m_model_builder;

    unsigned short m_vertex_attrib_bits;
    flatbuffers::Offset< flatbuffers::Vector<float> > m_vertex_data;
    flatbuffers::Offset< flatbuffers::Vector<GLuint> > m_indices;
    flatbuffers::Offset< flatbuffers::Vector<const Serialized::MeshMetaData*> > m_meshes_meta_data;

    std::unique_ptr<Serialized::MaterialBuilder> m_material_builder;
    flatbuffers::Offset<flatbuffers::String> m_albedo;
    flatbuffers::Offset<flatbuffers::String> m_roughness;
    flatbuffers::Offset<flatbuffers::String> m_ao;
    flatbuffers::Offset<flatbuffers::String> m_normal;
    flatbuffers::Offset<flatbuffers::String> m_specular;
    
    // need this to survive the duration of loading deserialized model into VAO
    std::unique_ptr<char[]> m_serialized_data;

public:
    ModelSerializer();
    ~ModelSerializer() = default;

    void AddModel(
        unsigned short vertex_attrib_bits,
        const float* vertex_data,
        size_t vertex_data_len,
        const GLuint* indices_data,
        size_t indices_data_len,
        const MeshMetaData* mesh_meta_data,
        size_t mesh_meta_data_len
    );
    void AddAlbedo(const FileSystem::Path& albedo_path);
    void AddRoughness(const FileSystem::Path& roughness_path);
    void AddAo(const FileSystem::Path& ao_path);
    void AddNormal(const FileSystem::Path& normal_path);
    void AddSpecular(const FileSystem::Path& specular_path);
    void Serialize(const FileSystem::Path& path_to_model);
    const Serialized::Model* GetDeserializedObject(const FileSystem::Path& path_to_model);

private:
    flatbuffers::Offset<Serialized::Material> FinishMaterial();
    flatbuffers::Offset<Serialized::Model> FinishModel(
        flatbuffers::Offset<Serialized::Material> material,
        uint64_t last_write_time
    );
};

HNCRSP_NAMESPACE_END