#include "Model.h"
#include "src/core/Texture2DManager.h"
#include "src/serialize/schemas/model_generated.h"


HNCRSP_NAMESPACE_START

Model::Model(const FileSystem::Path& path, std::shared_ptr<Shader> shader, bool flip_uv)
    : m_shader(shader)
{
    ModelSerializer modelSerializer;
    if (const Serialized::Model* deserialized_model = modelSerializer.GetDeserializedObject(path))
    {
        loadDeserializedModel(deserialized_model, shader);
        return;
    }

    // Model loading
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
    
    FileSystem::Path modelDirectory = path;
    modelDirectory.remove_filename();

    processNode(scene->mRootNode, scene, modelDirectory, modelSerializer);

    float beginTime2 = glfwGetTime();
    HNCRSP_LOG_INFO(glfwGetTime() - beginTime2);
    HNCRSP_LOG_INFO(  // display relative path from project directory
        fmt::format(
            "Model load time ~{}: {}s",
            path.relative_string(),
            glfwGetTime() - beginTime
        )
    );
    modelSerializer.Serialize(path);
}

void Model::processNode(
    aiNode* node,
    const aiScene* scene,
    const FileSystem::Path& modelDirectory,
    ModelSerializer& modelSerializer
) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene, modelDirectory, modelSerializer));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, modelDirectory, modelSerializer);
    }
}

Mesh Model::processMesh(
    aiMesh* mesh,
    const aiScene* scene,
    const FileSystem::Path& modelDirectory,
    ModelSerializer& modelSerializer
) {
    std::vector<glm::vec3> verticesPos;
    verticesPos.reserve(mesh->mNumVertices);

    std::vector<glm::vec3> normals;
    normals.reserve(mesh->mNumVertices);

    std::vector<glm::vec2> uvs;
    uvs.reserve(mesh->mNumVertices);

    std::vector<GLuint> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        verticesPos.emplace_back(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );
        normals.emplace_back(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );
        if (mesh->mTextureCoords[0])
        {
            uvs.emplace_back(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else
        {
            uvs.emplace_back(0.0f, 0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    EntityUID newMeshID = g_ECSManager->NewEntityUID();
    m_meshIDs.push_back(newMeshID);
    g_ECSManager->AddComponent<Transform>(newMeshID, {});  // dummy data to change later
    if (!m_material) m_material = std::make_shared<Material>(m_shader);

    if (mesh->mMaterialIndex >= 0 && m_material->getAlbedoMap() == nullptr)  // TODO: process multiple materials, only 1 material per model for now
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiString textureFilename;
        std::string texturePath;

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilename) != -1)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddAlbedo(FileSystem::Path(texturePath));
            Texture2D& diffuseMap = getMaterialTexture(texturePath.c_str(), aiTextureType_DIFFUSE);
            m_material->setAlbedoMap(diffuseMap);
        }
        if (material->GetTexture(aiTextureType_NORMALS, 0, &textureFilename) != -1)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddRoughness(FileSystem::Path(texturePath));
            Texture2D& normalMap = getMaterialTexture(texturePath.c_str(), aiTextureType_NORMALS);
            m_material->setNormalMap(normalMap);
        }
        if (material->GetTexture(aiTextureType_SPECULAR, 0, &textureFilename) != -1)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddSpecular(FileSystem::Path(texturePath));
            Texture2D& specularMap = getMaterialTexture(texturePath.c_str(), aiTextureType_SPECULAR);
            m_material->setSpecularMap(specularMap);
        }
        if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &textureFilename) != -1)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddRoughness(FileSystem::Path(texturePath));
            Texture2D& roughnessMap = getMaterialTexture(texturePath.c_str(), aiTextureType_DIFFUSE_ROUGHNESS);
            m_material->setRoughnessMap(roughnessMap);
        }
        if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &textureFilename) != -1)
        {
            texturePath = modelDirectory.string() + textureFilename.C_Str();
            modelSerializer.AddAo(FileSystem::Path(texturePath));
            Texture2D& aoMap = getMaterialTexture(texturePath.c_str(), aiTextureType_AMBIENT_OCCLUSION);
            m_material->setAoMap(aoMap);
        }
    }

    Mesh newMesh(
        &verticesPos,
        &indices,
        &normals,
        nullptr,
        &uvs
    );

    unsigned char vertex_attrib_bits = 
        VERTEX_ATTRIB_POSITION_BIT |
        VERTEX_ATTRIB_COLOR_BIT |
        VERTEX_ATTRIB_UV_BIT |
        VERTEX_ATTRIB_NORMAL_BIT;

    modelSerializer.AddMesh(
        vertex_attrib_bits,
        newMesh.GetVAO()->getData(),
        newMesh.GetVAO()->getDataLen(),
        indices.data(),
        indices.size()
    );

    return newMesh;
}

Texture2D& Model::getMaterialTexture(
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
    
    return g_Texture2DManager.getTexture2D(FileSystem::Path(texturePath), textureType, 1, 1);
}

void Model::loadDeserializedModel(
    const Serialized::Model* deserialized_model,
    std::shared_ptr<Shader> shader
) {
    // load all meshes in model
    m_meshes.reserve(deserialized_model->meshes()->size());
    m_meshIDs.reserve(deserialized_model->meshes()->size());
    for (unsigned int i = 0; i < deserialized_model->meshes()->size(); i++)
    {
        auto mesh = deserialized_model->meshes()->Get(i);
        m_meshes.emplace_back(
            mesh->vertex_attrib_bits(),
            mesh->vertex_data()->data(),
            mesh->vertex_data()->size(),
            mesh->indices()->data(),
            mesh->indices()->size()
        );

        EntityUID newMeshID = g_ECSManager->NewEntityUID();
        m_meshIDs.push_back(newMeshID);
        g_ECSManager->AddComponent<Transform>(newMeshID, {});  // dummy data to change later
    }

    // load material
    auto material = deserialized_model->material();
    m_shader = shader;
    m_material = std::make_shared<Material>(shader);
    std::string_view albedo = material->albedo_path()->string_view();
    std::string_view roughness = material->specular_path()->string_view();
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

void Model::setAllMeshTransform(const Transform& transform) const
{
    for (const EntityUID& entityUID : m_meshIDs)
    {
        g_ECSManager->GetComponent<Transform>(entityUID) = transform;
    }
}

void Model::virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material)
{
    for (size_t i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].virt_AddMeshDataToRenderer(m_meshIDs[i], m_material);
    }
}

Material* Model::getMaterial() const
{
    return m_material.get();
}

HNCRSP_NAMESPACE_END