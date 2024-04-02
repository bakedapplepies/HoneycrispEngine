#pragma once

#include "src/pch/pch.h"
#include "src/serialize/Serializer.h"
#include "src/serialize/schemas/model_generated.h"


HNCRSP_NAMESPACE_START

class ModelSerializer : public Serializer<Serialized::Model>
{
private:
    flatbuffers::FlatBufferBuilder m_builder;// = flatbuffers::FlatBufferBuilder(1024);
    std::vector< flatbuffers::Offset<Serialized::Mesh> > m_meshes;
    std::unique_ptr<Serialized::MaterialBuilder> m_material_builder;

    flatbuffers::Offset<flatbuffers::String> m_albedo;
    flatbuffers::Offset<flatbuffers::String> m_roughness;
    flatbuffers::Offset<flatbuffers::String> m_ao;
    flatbuffers::Offset<flatbuffers::String> m_normal;
    flatbuffers::Offset<flatbuffers::String> m_specular;

    std::unique_ptr<char[]> m_serialized_data;

public:
    ModelSerializer();
    ~ModelSerializer();

    void AddMesh(
        unsigned char vertex_attrib_bits,
        const float* vertex_data,
        size_t vertex_data_len,
        const GLuint* indices_data,
        size_t indices_data_len
    );
    void AddAlbedo(const FileSystem::Path& albedo_path);
    void AddRoughness(const FileSystem::Path& roughness_path);
    void AddAo(const FileSystem::Path& ao_path);
    void AddNormal(const FileSystem::Path& normal_path);
    void AddSpecular(const FileSystem::Path& specular_path);
    void Serialize(const FileSystem::Path& path_to_serialized_obj);
    const Serialized::Model* GetDeserializedObject(const FileSystem::Path& path_to_serialized_obj);

private:
    flatbuffers::Offset<Serialized::Material> FinishMaterial();
};

HNCRSP_NAMESPACE_END