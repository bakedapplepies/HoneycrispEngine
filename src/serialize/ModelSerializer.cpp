#include "ModelSerializer.h"


HNCRSP_NAMESPACE_START

ModelSerializer::ModelSerializer()
{
    m_albedo = m_builder.CreateString("");  // dummy data
    m_roughness = m_builder.CreateString("");
    m_ao = m_builder.CreateString("");
    m_normal = m_builder.CreateString("");
    m_specular = m_builder.CreateString("");
}

ModelSerializer::~ModelSerializer()
{

}

void ModelSerializer::AddMesh(
    unsigned char vertex_attrib_bits,
    const float* vertex_data,
    size_t vertex_data_len,
    const GLuint* indices_data,
    size_t indices_data_len
) {
    auto serialized_vertex_data = m_builder.CreateVector(vertex_data, vertex_data_len);
    auto serialized_indices = m_builder.CreateVector(indices_data, indices_data_len);

    m_meshes.push_back(Serialized::CreateMesh(
        m_builder,
        vertex_attrib_bits,
        serialized_vertex_data,
        serialized_indices
    ));
}

void ModelSerializer::AddAlbedo(const FileSystem::Path& albedo_path)
{
    m_albedo = m_builder.CreateString(albedo_path.string().c_str());
}

void ModelSerializer::AddRoughness(const FileSystem::Path& roughness_path)
{
    m_roughness = m_builder.CreateString(roughness_path.string().c_str());
}

void ModelSerializer::AddAo(const FileSystem::Path& ao_path)
{
    m_ao = m_builder.CreateString(ao_path.string().c_str());
}

void ModelSerializer::AddNormal(const FileSystem::Path& normal_path)
{
    m_normal = m_builder.CreateString(normal_path.string().c_str());
}

void ModelSerializer::AddSpecular(const FileSystem::Path& specular_path)
{
    m_specular = m_builder.CreateString(specular_path.string().c_str());
}

flatbuffers::Offset<Serialized::Material> ModelSerializer::FinishMaterial()
{
    m_material_builder = std::make_unique<Serialized::MaterialBuilder>(m_builder);
    
    m_material_builder->add_albedo_path(m_albedo);
    m_material_builder->add_roughness_path(m_roughness);
    m_material_builder->add_ao_path(m_ao);
    m_material_builder->add_normal_path(m_normal);
    m_material_builder->add_specular_path(m_specular);
    return m_material_builder->Finish();
}

void ModelSerializer::Serialize(const FileSystem::Path& path_to_model)
{
    std::filesystem::create_directories("serialized/models");
    int64_t path_hash = std::hash<std::string>{}(path_to_model.string());
    std::string serialized_model_path = fmt::format("serialized/models/{}.hncmdl", path_hash);

    auto last_write_time_since_epoch =
        std::filesystem::last_write_time(path_to_model.string()).time_since_epoch();
    auto time_count = std::chrono::duration_cast<std::chrono::seconds>(last_write_time_since_epoch);

    auto serialized_material = FinishMaterial();
    auto serialized_meshes = m_builder.CreateVector(m_meshes);
    auto serialized_model = Serialized::CreateModel(m_builder, serialized_meshes, serialized_material, time_count.count());
    m_builder.Finish(serialized_model);

    // store buffer
    void* buffer = m_builder.GetBufferPointer();
    unsigned int buffer_size = m_builder.GetSize();
    std::ofstream outFile(serialized_model_path, std::ios::out | std::ios::binary | std::ios::trunc);
    outFile.write((char*)buffer, buffer_size);
    outFile.close();
}

const Serialized::Model* ModelSerializer::GetDeserializedObject(const FileSystem::Path& path_to_model)
{
    int64_t path_hash = std::hash<std::string>{}(path_to_model.string());
    std::string serialized_model_path = fmt::format("serialized/models/{}.hncmdl", path_hash);
    if(!std::filesystem::exists(serialized_model_path))
    {
        return nullptr;
    }

    auto last_write_time_since_epoch =
        std::filesystem::last_write_time(path_to_model.string()).time_since_epoch();
    auto time_count = std::chrono::duration_cast<std::chrono::seconds>(last_write_time_since_epoch);

    // read serialized data
    std::ifstream inFile(serialized_model_path, std::ios::in | std::ios::binary);
    inFile.seekg(0, std::ios::end);
    unsigned int length = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    m_serialized_data = std::make_unique<char[]>(length);
    inFile.read(m_serialized_data.get(), length);
    inFile.close();

    const Serialized::Model* serialized_model = Serialized::GetModel(m_serialized_data.get());

    if (serialized_model->last_write_time() != time_count.count())
    {
        return nullptr;
    }
    
    return serialized_model;
}

HNCRSP_NAMESPACE_END