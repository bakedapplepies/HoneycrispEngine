#include "ModelSerializer.h"

HNCRSP_NAMESPACE_START

ModelSerializer::ModelSerializer()
{}

void ModelSerializer::AddModel(
    uint8_t vertex_attrib_bits,
    const float* vertex_data,
    size_t vertex_data_len,
    const GLuint* indices_data,
    size_t indices_data_len,
    const MeshMetaData* mesh_meta_data,
    size_t mesh_meta_data_len
) {
    m_vertex_attrib_bits = vertex_attrib_bits;

    std::vector<Serialized::MeshMetaData> mesh_meta_data_vec;
    m_vertex_data = m_builder.CreateVector(vertex_data, vertex_data_len);
    m_indices = m_builder.CreateVector(indices_data, indices_data_len);

    for (unsigned int i = 0; i < mesh_meta_data_len; i++)
    {
        mesh_meta_data_vec.emplace_back(
            mesh_meta_data[i].mesh_vertex_offset,
            mesh_meta_data[i].indices_buffer_count,
            mesh_meta_data[i].material_index
        );
    }
    m_meshes_meta_data = m_builder.CreateVectorOfStructs(mesh_meta_data_vec);
}

void ModelSerializer::AddMaterial(
    const FileSystem::Path& albedo_path,
    const FileSystem::Path& roughness_path,
    const FileSystem::Path& ao_path,
    const FileSystem::Path& normal_path,
    const FileSystem::Path& specular_path
) {
    flatbuffers::Offset<flatbuffers::String> albedo = (albedo_path != FileSystem::Path("")) ? 
        m_builder.CreateString(albedo_path.string()) : m_builder.CreateString("");
    flatbuffers::Offset<flatbuffers::String> roughness = (roughness_path != FileSystem::Path("")) ? 
        m_builder.CreateString(roughness_path.string()) : m_builder.CreateString("");
    flatbuffers::Offset<flatbuffers::String> ao = (ao_path != FileSystem::Path("")) ? 
        m_builder.CreateString(ao_path.string()) : m_builder.CreateString("");
    flatbuffers::Offset<flatbuffers::String> normal = (normal_path != FileSystem::Path("")) ? 
        m_builder.CreateString(normal_path.string()) : m_builder.CreateString("");
    flatbuffers::Offset<flatbuffers::String> specular = (specular_path != FileSystem::Path("")) ? 
        m_builder.CreateString(specular_path.string()) : m_builder.CreateString("");

    m_materials.push_back(Serialized::CreateMaterial(
        m_builder,
        albedo,
        roughness,
        ao,
        normal,
        specular
    ));
}

flatbuffers::Offset<Serialized::Model> ModelSerializer::_FinishModel(
    uint64_t last_write_time
) {
    auto materials = m_builder.CreateVector<Serialized::Material>(m_materials.data(), m_materials.size());

    m_model_builder = std::make_unique<Serialized::ModelBuilder>(m_builder);

    m_model_builder->add_vertex_attrib_bits(m_vertex_attrib_bits);
    m_model_builder->add_vertex_data(m_vertex_data);
    m_model_builder->add_indices(m_indices);
    m_model_builder->add_meshes(m_meshes_meta_data);
    m_model_builder->add_materials(materials);
    m_model_builder->add_last_write_time(last_write_time);

    return m_model_builder->Finish();
}

void ModelSerializer::Serialize(const FileSystem::Path& path_to_model)
{
    // Get hash from path
    FileSystem::Path serialized_dir(fmt::format("build/{}/src/serialized/models", HNCRSP_BUILD_TYPE));
    std::filesystem::create_directories(serialized_dir.string());
    int64_t path_hash = std::hash<std::string>{}(path_to_model.string());
    std::string serialized_model_path = fmt::format("{}/{}.hncmdl", serialized_dir.string(), path_hash);

    // Get last write time
    auto last_write_time_since_epoch =
        std::filesystem::last_write_time(path_to_model.string()).time_since_epoch();
    auto time_count = std::chrono::duration_cast<std::chrono::seconds>(last_write_time_since_epoch);

    // Finish material & model
    auto serialized_model = _FinishModel(time_count.count());
    m_builder.Finish(serialized_model);

    // store buffer
    void* buffer = m_builder.GetBufferPointer();
    uint32_t buffer_size = m_builder.GetSize();

    std::ofstream outFile(serialized_model_path, std::ios::out | std::ios::binary | std::ios::trunc);
    outFile.write(reinterpret_cast<char*>(buffer), buffer_size);
    outFile.close();
}

const Serialized::Model* ModelSerializer::GetDeserializedObject(const FileSystem::Path& path_to_model)
{
    std::string serialized_model_path;
    if (path_to_model.extension() == ".hncmdl")
    {
        serialized_model_path = path_to_model.string();
    }
    else
    {
        int64_t path_hash = std::hash<std::string>{}(path_to_model.string());
        serialized_model_path = FileSystem::Path(fmt::format("build/{}/src/serialized/models/{}.hncmdl", HNCRSP_BUILD_TYPE, path_hash)).string();
        if(!std::filesystem::exists(serialized_model_path))
        {
            return nullptr;
        }
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

    const Serialized::Model* deserialized_model = Serialized::GetModel(m_serialized_data.get());

    if (deserialized_model->last_write_time() != time_count.count())
    {
        return nullptr;
    }
    
    return deserialized_model;
}

HNCRSP_NAMESPACE_END