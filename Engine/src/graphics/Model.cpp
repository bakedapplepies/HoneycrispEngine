#include "api/graphics/Model.h"
#include "api/managers/Texture2DManager.h"
#include "api/ecs/ECSManager.h"
#include "api/serialization/schemas/model_generated.h"

HNCRSP_NAMESPACE_START

static bool s_isFirstMatNull = false;

Model::Model(const FileSystem::Path& path, const Shader* shader, bool flip_uv)
{
    ModelSerializer modelSerializer;
    if (const Serialized::Model* deserialized_model = modelSerializer.GetDeserializedObject(path))
    {
        _LoadDeserializedModel(deserialized_model, shader);
        return;
    }

    // Model loading ----------
    float beginTime = glfwGetTime();

    Assimp::Importer import;
    const aiScene*  scene = import.ReadFile(path.string().data(),
        aiProcess_Triangulate
        | aiProcess_FlipWindingOrder
        | aiProcess_CalcTangentSpace
        | (flip_uv ? aiProcess_FlipUVs : 0)
        // | aiProcess_GenNormals
    );
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HNCRSP_TERMINATE("ASSIMP: {}", import.GetErrorString());
    }
    
    // Process Model ----------
    FileSystem::Path modelDirectory = path;
    modelDirectory.remove_filename();
    std::vector<Vertex> vertexData;
    std::vector<GLuint> indices;

    _GetMaterials(scene, modelDirectory, modelSerializer, shader);

    // HNCRSP_INFO(scene->mNumMaterials);
    
    _ProcessNode(scene->mRootNode, scene, modelDirectory, modelSerializer, vertexData, indices);

    HNCRSP_INFO(  // display relative path from project directory
        "Model load time ~{}: {}s",
        path.relative_string(),
        glfwGetTime() - beginTime
    );

    // Vertex Array construction ----------
    uint16_t vertex_attrib_bits =
        VERTEX_ATTRIB_POSITION_BIT
        | VERTEX_ATTRIB_UV_BIT
        | VERTEX_ATTRIB_NORMAL_BIT
        | VERTEX_ATTRIB_TANGENT_BIT;

    m_VAO = VertexArray(
        vertex_attrib_bits,
        &vertexData,
        &indices
    );

    // Serialization ----------
    modelSerializer.AddModel(
        vertex_attrib_bits,
        vertexData,
        indices,
        m_meshesMetaData
    );
    modelSerializer.Serialize(path);

    s_isFirstMatNull = false;  // for next model
}

void Model::_ProcessNode(
    aiNode* node,
    const aiScene* scene,
    const FileSystem::Path& model_directory,
    ModelSerializer& modelSerializer,
    std::vector<Vertex>& vertexData,
    std::vector<GLuint>& indices
) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _ProcessMesh(mesh, scene, model_directory, modelSerializer, vertexData, indices);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        _ProcessNode(node->mChildren[i], scene, model_directory, modelSerializer, vertexData, indices);
    }
}

void Model::_ProcessMesh(
    aiMesh* mesh,
    const aiScene* scene,
    const FileSystem::Path& model_directory,
    ModelSerializer& modelSerializer,
    std::vector<Vertex>& vertexData,
    std::vector<GLuint>& indices
) {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        glm::vec2 uv = (mesh->mTextureCoords[0])
            ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
            : glm::vec2(0.0f, 0.0f);
        glm::vec3 tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        
        vertexData.emplace_back(Vertex {
            .position = position,
            .normal = normal,
            .uv = uv,
            .tangent = tangent
        });
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    static uint32_t num_vertices;

    if (m_meshesMetaData.size() == 0)
    {
        m_meshesMetaData.push_back(MeshMetaData {
            .mesh_vertex_offset = 0,
            .indices_buffer_count = mesh->mNumFaces * 3,
            .material_index = mesh->mMaterialIndex - static_cast<uint32_t>(s_isFirstMatNull) 
        });
    }
    else
    {
        m_meshesMetaData.push_back(MeshMetaData {
            .mesh_vertex_offset = m_meshesMetaData.back().mesh_vertex_offset + num_vertices,
            .indices_buffer_count = mesh->mNumFaces * 3,
            .material_index = mesh->mMaterialIndex - static_cast<uint32_t>(s_isFirstMatNull)
        });
    }
    num_vertices = mesh->mNumVertices;
}

void Model::_GetMaterials(
    const aiScene* scene,
    const FileSystem::Path& model_directory,
    ModelSerializer& modelSerializer,
    const Shader* shader
) {
    for (uint32_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i];

        if (i == 0)
        {
            uint32_t tex_count = material->GetTextureCount(aiTextureType_DIFFUSE);
            if (tex_count == 0)
            {
                s_isFirstMatNull = true;
                continue;
            }
        }
        
        Material currentMaterial(shader);

        aiString textureFilename;
        std::string texturePath;

        FileSystem::Path albedo("");
        FileSystem::Path roughness("");
        FileSystem::Path ao("");
        FileSystem::Path normal("");
        FileSystem::Path specular("");

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = model_directory.string() + textureFilename.C_Str();
            albedo = FileSystem::Path(texturePath);

            currentMaterial.SetAlbedoMap(albedo);
        }
        if (material->GetTexture(aiTextureType_NORMALS, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = model_directory.string() + textureFilename.C_Str();
            normal = FileSystem::Path(texturePath);

            currentMaterial.SetNormalMap(normal);
        }
        if (material->GetTexture(aiTextureType_SPECULAR, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = model_directory.string() + textureFilename.C_Str();
            specular = FileSystem::Path(texturePath);

            currentMaterial.SetSpecularMap(specular);
        }
        if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = model_directory.string() + textureFilename.C_Str();
            roughness = FileSystem::Path(texturePath);

            currentMaterial.SetRoughnessMap(roughness);
        }
        if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = model_directory.string() + textureFilename.C_Str();
            ao = FileSystem::Path(texturePath);

            currentMaterial.SetAoMap(ao);
        }

        m_materials.push_back(currentMaterial);

        modelSerializer.AddMaterial(
            albedo,
            roughness,
            ao,
            normal,
            specular
        );
    }
}

const Texture2D* Model::_GetMaterialTexture(
    std::string_view texturePath,
    aiTextureType assimp_texture_type
) {
    ETextureType textureType = ETextureType::NONE;
    switch (assimp_texture_type)
    {
    case aiTextureType_DIFFUSE:
        textureType = ETextureType::ALBEDO;
        break;

    case aiTextureType_DIFFUSE_ROUGHNESS:
        textureType = ETextureType::ROUGHNESS;
        break;

    case aiTextureType_AMBIENT_OCCLUSION:
        textureType = ETextureType::AO;
        break;

    case aiTextureType_NORMALS:
        textureType = ETextureType::NORMAL;
        break;

    case aiTextureType_SPECULAR:
        textureType = ETextureType::SPECULAR;
        break;

    default:
        HNCRSP_TERMINATE("Texture2D type not recognized.");
        break;
    }
    
    return g_Texture2DManager.GetTexture2D(FileSystem::Path(texturePath), textureType);
}

void Model::_LoadDeserializedModel(const Serialized::Model* deserialized_model, const Shader* shader)
{
    std::vector<float> vertex_data = std::vector<float>(
        deserialized_model->vertex_data()->begin(),
        deserialized_model->vertex_data()->end()
    );
    std::vector<GLuint> indices = std::vector<GLuint>(
        deserialized_model->indices()->begin(),
        deserialized_model->indices()->end()
    );

    m_VAO = VertexArray(
        deserialized_model->vertex_attrib_bits(),
        vertex_data,
        indices
    );

    m_meshesMetaData.reserve(deserialized_model->meshes()->size());
    for (unsigned int i = 0; i < deserialized_model->meshes()->size(); i++)
    {
        m_meshesMetaData.emplace_back(
            deserialized_model->meshes()->Get(i)->mesh_vertex_offset(),
            deserialized_model->meshes()->Get(i)->indices_buffer_count(),
            deserialized_model->meshes()->Get(i)->material_index()
        );
    }

    // load material
    auto materials = deserialized_model->materials();

    for (uint32_t i = 0; i < materials->size(); i++)
    {
        auto material = (*materials)[i];
        std::string_view albedo = material->albedo_path()->string_view();
        std::string_view roughness = material->roughness_path()->string_view();
        std::string_view ao = material->ao_path()->string_view();
        std::string_view normal = material->normal_path()->string_view();
        std::string_view specular = material->specular_path()->string_view();

        Material currentMaterial(shader);

        if (albedo != "") currentMaterial.SetAlbedoMap(
            FileSystem::Path(albedo)
        );
        if (roughness != "") currentMaterial.SetRoughnessMap(
            FileSystem::Path(roughness)
        );
        if (ao != "") currentMaterial.SetAoMap(
            FileSystem::Path(ao)
        );
        if (normal != "") currentMaterial.SetNormalMap(
            FileSystem::Path(normal)
        );
        if (specular != "") currentMaterial.SetSpecularMap(
            FileSystem::Path(specular)
        );

        m_materials.push_back(currentMaterial);
    }
}

void Model::virt_AddDrawDataToRenderer(ECS::EntityUID entityUID, const Material& material) const
{
    DrawData meshData;
    meshData.VAO_id = m_VAO.GetID();
    meshData.meta_data = std::vector<MeshMetaData>(m_meshesMetaData.begin(), m_meshesMetaData.end());
    meshData.materials = std::vector<Material>(
        m_materials.begin(),
        m_materials.end()
    );

    g_ECSManager.AddComponent<DrawData>(entityUID, meshData);

    m_meshesMetaData.clear();
    m_materials.clear();
}

Model::Model(Model&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
    m_meshesMetaData = std::move(other.m_meshesMetaData);
    m_materials = std::move(other.m_materials);
}

Model& Model::operator=(Model&& other) noexcept
{
    m_VAO = std::move(other.m_VAO);
    m_meshesMetaData = std::move(other.m_meshesMetaData);
    m_materials = std::move(other.m_materials);

    return *this;
}

HNCRSP_NAMESPACE_END