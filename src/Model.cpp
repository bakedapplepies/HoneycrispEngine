#include "Model.h"
#include "src/managers/Texture2DManager.h"
#include "src/managers/ShaderManager.h"
#include "src/ecs/ECSManager.h"
#include "src/serialize/schemas/model_generated.h"


HNCRSP_NAMESPACE_START

static uint32_t vertex_count = 0;

Model::Model(const FileSystem::Path& path, std::shared_ptr<Shader> shader, bool flip_uv)
{
    m_material = std::make_shared<Material>(shader);

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
        aiProcess_Triangulate |
        aiProcess_FlipWindingOrder |
        (flip_uv ? aiProcess_FlipUVs : 0) |
        aiProcess_GenNormals
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
        VERTEX_ATTRIB_POSITION_BIT |
        VERTEX_ATTRIB_UV_BIT |
        VERTEX_ATTRIB_NORMAL_BIT;

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
        vertex_count++;
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
            0,                      // index offset
            mesh->mNumFaces * 3     // vertex count
        );
    }
    else
    {
        m_meshesMetaData.emplace_back(
            m_meshesMetaData.back().index_offset + num_vertices,
            mesh->mNumFaces * 3
        );
    }

    num_vertices = mesh->mNumVertices;

    // latter check to ensure this only runs once
    if (mesh->mMaterialIndex >= 0 && m_material->getAlbedoMap() == nullptr)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiString textureFilename;   
        std::string texturePath;

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddAlbedo(FileSystem::Path(texturePath));

            std::shared_ptr<Texture2D> diffuseMap =
                getMaterialTexture(texturePath.c_str(), aiTextureType_DIFFUSE);
            m_material->setAlbedoMap(diffuseMap);
        }
        if (material->GetTexture(aiTextureType_NORMALS, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddRoughness(FileSystem::Path(texturePath));

            std::shared_ptr<Texture2D> normalMap =
                getMaterialTexture(texturePath.c_str(), aiTextureType_NORMALS);
            m_material->setNormalMap(normalMap);
        }
        if (material->GetTexture(aiTextureType_SPECULAR, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddSpecular(FileSystem::Path(texturePath));

            std::shared_ptr<Texture2D> specularMap =
                getMaterialTexture(texturePath.c_str(), aiTextureType_SPECULAR);
            m_material->setSpecularMap(specularMap);
        }
        if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddRoughness(FileSystem::Path(texturePath));

            std::shared_ptr<Texture2D> roughnessMap =
                getMaterialTexture(texturePath.c_str(), aiTextureType_DIFFUSE_ROUGHNESS);
            m_material->setRoughnessMap(roughnessMap);
        }
        if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &textureFilename) != aiReturn_FAILURE)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddAo(FileSystem::Path(texturePath));

            std::shared_ptr<Texture2D> aoMap =
                getMaterialTexture(texturePath.c_str(), aiTextureType_AMBIENT_OCCLUSION);
            m_material->setAoMap(aoMap);
        }
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
    m_meshesMetaData.reserve(deserialized_model->meshes()->size());
    for (unsigned int i = 0; i < deserialized_model->meshes()->size(); i++)
    {
        m_meshesMetaData.emplace_back(
            deserialized_model->meshes()->Get(i)->base_index(),
            deserialized_model->meshes()->Get(i)->vertex_count()
        );
    }

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

    // load material
    auto material = deserialized_model->material();
    
    std::string_view albedo = material->albedo_path()->string_view();
    std::string_view roughness = material->roughness_path()->string_view();
    std::string_view ao = material->ao_path()->string_view();
    std::string_view normal = material->normal_path()->string_view();
    std::string_view specular = material->specular_path()->string_view();

    if (albedo != "") m_material->setAlbedoMap(
        FileSystem::Path(albedo)
    );
    if (roughness != "") m_material->setRoughnessMap(
        FileSystem::Path(roughness)
    );
    if (ao != "") m_material->setAoMap(
        FileSystem::Path(ao)
    );
    if (normal != "") m_material->setNormalMap(
        FileSystem::Path(normal)
    );
    if (specular != "") m_material->setSpecularMap(
        FileSystem::Path(specular)
    );
}

void Model::virt_AddDrawDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material)
{
    DrawData meshData;
    meshData.VAO_id = m_VAO->getID();
    meshData.meta_data = std::vector<MeshMetaData>(m_meshesMetaData.begin(), m_meshesMetaData.end());
    // if (material)  // TODO: what to do about these comments
    // {
    //     meshData.material = material;
    //     HNCRSP_LOG_INFO(meshData.material->getShader()->getID());
    // }
    // else
    // {
    //     meshData.material = std::make_shared<Material>(g_ShaderManager.basicShader);
    //     HNCRSP_LOG_INFO(meshData.material->getShader()->getID());
    // }
    meshData.material = m_material;

    g_ECSManager->AddComponent<DrawData>(entityUID, meshData);
}

Material* Model::getMaterial() const
{
    return m_material.get();
}

HNCRSP_NAMESPACE_END