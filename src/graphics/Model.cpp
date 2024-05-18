#include "Model.h"
#include "src/managers/Texture2DManager.h"
#include "src/managers/ShaderManager.h"
#include "src/ecs/ECSManager.h"
#include "src/serialize/schemas/model_generated.h"


HNCRSP_NAMESPACE_START

static bool s_isFirstMatNull = false;

Model::Model(const FileSystem::Path& path, std::shared_ptr<Shader> shader, bool flip_uv)
{
    m_shader = shader;
    ModelSerializer modelSerializer;
    if (const Serialized::Model* deserialized_model = modelSerializer.GetDeserializedObject(path))
    {
        loadDeserializedModel(deserialized_model);
        return;
    }

    // Model loading ----------
    float beginTime = glfwGetTime();

    Assimp::Importer import;
    const aiScene*  scene = import.ReadFile(path.string().data(),
        aiProcess_Triangulate
        | aiProcess_FlipWindingOrder
        | (flip_uv ? aiProcess_FlipUVs : 0)
        // | aiProcess_GenNormals
    );
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HNCRSP_TERMINATE(fmt::format("ASSIMP: {}", import.GetErrorString()).c_str());
    }
    
    // Process Model ----------
    FileSystem::Path modelDirectory = path;
    modelDirectory.remove_filename();
    std::vector<float> vertexData;
    std::vector<GLuint> indices;

    getMaterials(scene, modelDirectory, modelSerializer);

    HNCRSP_LOG_INFO(scene->mNumMaterials);
    
    processNode(scene->mRootNode, scene, modelDirectory, modelSerializer, vertexData, indices);

    HNCRSP_LOG_INFO(  // display relative path from project directory
        fmt::format(
            "Model load time ~{}: {}s",
            path.relative_string(),
            glfwGetTime() - beginTime
        )
    );

    // Vertex Array construction ----------
    unsigned short vertex_attrib_bits =
        VERTEX_ATTRIB_POSITION_BIT
        | VERTEX_ATTRIB_UV_BIT
        | VERTEX_ATTRIB_NORMAL_BIT;

    m_VAO = std::make_unique<VertexArray>(
        vertex_attrib_bits,
        vertexData,
        indices
    );

    // Serialization ----------
    modelSerializer.AddModel(
        vertex_attrib_bits,
        vertexData.data(),
        vertexData.size(),
        indices.data(),
        indices.size(),
        m_meshesMetaData.data(),
        m_meshesMetaData.size()
    );
    modelSerializer.Serialize(path);

    s_isFirstMatNull = false;  // for next model
}

void Model::processNode(
    aiNode* node,
    const aiScene* scene,
    const FileSystem::Path& modelDirectory,
    ModelSerializer& modelSerializer,
    std::vector<float>& vertexData,
    std::vector<GLuint>& indices
) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, modelDirectory, modelSerializer, vertexData, indices);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, modelDirectory, modelSerializer, vertexData, indices);
    }
}

void Model::processMesh(
    aiMesh* mesh,
    const aiScene* scene,
    const FileSystem::Path& modelDirectory,
    ModelSerializer& modelSerializer,
    std::vector<float>& vertexData,
    std::vector<GLuint>& indices
) {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Order is important here
        // Position -> UV -> Normal
        vertexData.push_back(mesh->mVertices[i].x);
        vertexData.push_back(mesh->mVertices[i].y);
        vertexData.push_back(mesh->mVertices[i].z);

        if (mesh->mTextureCoords[0])
        {
            vertexData.push_back(mesh->mTextureCoords[0][i].x);
            vertexData.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertexData.push_back(0.0f);
            vertexData.push_back(0.0f);
        }

        vertexData.push_back(mesh->mNormals[i].x);
        vertexData.push_back(mesh->mNormals[i].y);
        vertexData.push_back(mesh->mNormals[i].z);
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
        m_meshesMetaData.emplace_back(
            0,                                                               // index offset
            mesh->mNumFaces * 3,                                             // vertex count
            mesh->mMaterialIndex - static_cast<uint32_t>(s_isFirstMatNull)   // material index
        );
    }
    else
    {
        m_meshesMetaData.emplace_back(
            m_meshesMetaData.back().mesh_vertex_count + num_vertices,
            mesh->mNumFaces * 3,
            mesh->mMaterialIndex - static_cast<uint32_t>(s_isFirstMatNull)
        );
    }
    num_vertices = mesh->mNumVertices;
}

void Model::getMaterials(
    const aiScene* scene,
    const FileSystem::Path& modelDirectory,
    ModelSerializer& modelSerializer
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
        
        m_materials.push_back(std::make_shared<Material>(m_shader));
        std::shared_ptr<Material>& currentMat = m_materials.back();

        aiString textureFilename;
        std::string texturePath;

        FileSystem::Path albedo("");
        FileSystem::Path roughness("");
        FileSystem::Path ao("");
        FileSystem::Path normal("");
        FileSystem::Path specular("");

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            albedo = FileSystem::Path(texturePath);

            currentMat->setAlbedoMap(albedo);
        }
        if (material->GetTexture(aiTextureType_NORMALS, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            normal = FileSystem::Path(texturePath);

            currentMat->setNormalMap(normal);
        }
        if (material->GetTexture(aiTextureType_SPECULAR, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            specular = FileSystem::Path(texturePath);

            currentMat->setSpecularMap(specular);
        }
        if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            roughness = FileSystem::Path(texturePath);

            currentMat->setRoughnessMap(roughness);
        }
        if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            ao = FileSystem::Path(texturePath);

            currentMat->setAoMap(ao);
        }

        modelSerializer.AddMaterial(
            albedo,
            roughness,
            ao,
            normal,
            specular
        );
    }
}

std::shared_ptr<Texture2D> Model::getMaterialTexture(
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

void Model::loadDeserializedModel(const Serialized::Model* deserialized_model)
{
    std::vector<float> vertex_data = std::vector<float>(
        deserialized_model->vertex_data()->begin(),
        deserialized_model->vertex_data()->end()
    );
    std::vector<GLuint> indices = std::vector<GLuint>(
        deserialized_model->indices()->begin(),
        deserialized_model->indices()->end()
    );

    m_VAO = std::make_unique<VertexArray>(
        deserialized_model->vertex_attrib_bits(),
        vertex_data,
        indices
    );

    m_meshesMetaData.reserve(deserialized_model->meshes()->size());
    for (unsigned int i = 0; i < deserialized_model->meshes()->size(); i++)
    {
        m_meshesMetaData.emplace_back(
            deserialized_model->meshes()->Get(i)->mesh_vertex_count(),
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

        m_materials.push_back(std::make_shared<Material>(m_shader));
        std::shared_ptr<Material>& currentMat = m_materials.back();

        if (albedo != "") currentMat->setAlbedoMap(
            FileSystem::Path(albedo)
        );
        if (roughness != "") currentMat->setRoughnessMap(
            FileSystem::Path(roughness)
        );
        if (ao != "") currentMat->setAoMap(
            FileSystem::Path(ao)
        );
        if (normal != "") currentMat->setNormalMap(
            FileSystem::Path(normal)
        );
        if (specular != "") currentMat->setSpecularMap(
            FileSystem::Path(specular)
        );
    }
}

void Model::virt_AddDrawDataToRenderer(EntityUID entityUID)
{
    DrawData meshData;
    meshData.VAO_id = m_VAO->getID();
    meshData.meta_data = std::vector<MeshMetaData>(m_meshesMetaData.begin(), m_meshesMetaData.end());
    meshData.materials = std::vector< std::shared_ptr<Material> >(
        m_materials.begin(),
        m_materials.end()
    );

    g_ECSManager->AddComponent<DrawData>(entityUID, meshData);
}

HNCRSP_NAMESPACE_END