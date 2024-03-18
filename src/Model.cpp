#include "Model.h"
#include "src/core/Texture2DManager.h"


HNCRSP_NAMESPACE_START

Model::Model(const FileSystem::Path& path, std::shared_ptr<Shader> shader, bool flip_uv)
    : m_shader(shader)
{
    // Model loading
    float beginTime = glfwGetTime();
    Assimp::Importer import;
    const aiScene*  scene = import.ReadFile(path.string().data(), aiProcess_Triangulate | aiProcess_FlipWindingOrder | (flip_uv ? aiProcess_FlipUVs : 0));

    // std::filesystem::create_directories("cache/models");
    // std::ofstream outFile(fmt::format("cache/models/{}.dat", std::hash<std::string>{}(path.string())), std::ios::out | std::ios::binary | std::ios::trunc);
    // std::ifstream inFile(path.string());
    // std::string line;
    // while (getline(inFile, line))
    // {
    //     outFile.write(line.c_str(), sizeof(const char*));
    // }
    // outFile.close();
    // to read from binary source, parser has to be handmade

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HNCRSP_TERMINATE(fmt::format("ASSIMP: {}", import.GetErrorString()).c_str());
    }
    m_modelDirectory = std::filesystem::path(path.string());
    m_modelDirectory = m_modelDirectory.remove_filename();

    std::vector< std::future< std::future<Mesh> > > future_meshes;
    processNode(scene->mRootNode, scene, future_meshes);
    m_meshes.reserve(future_meshes.size());
    float beginTime2 = glfwGetTime();
    for (unsigned int i = 0; i < future_meshes.size(); i++)
    {
        m_meshes.push_back(future_meshes[i].get().get());
    }
    HNCRSP_LOG_INFO(glfwGetTime() - beginTime2);
    HNCRSP_LOG_INFO(  // display relative path from project directory
        fmt::format(
            "Model load time ~{}: {}s",
            path.relative_string(),
            glfwGetTime() - beginTime
        )
    );
}

void Model::processNode(aiNode* node, const aiScene* scene, std::vector< std::future< std::future<Mesh> > >& future_meshes)
{
    // std::vector< std::future< std::future<Mesh> > > future_meshes;
    future_meshes.reserve(node->mNumMeshes);
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        future_meshes.emplace_back(std::async(std::launch::async, [this, mesh, scene] {
            return processMesh(mesh, scene);
        }));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, future_meshes);
    }
}

std::future<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<glm::vec3> verticesPos;
    verticesPos.reserve(mesh->mNumVertices);
    // std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    normals.reserve(mesh->mNumVertices);
    std::vector<glm::vec2> uvs;
    uvs.reserve(mesh->mNumVertices);
    std::vector<GLuint> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        verticesPos.push_back(glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        ));
        normals.push_back(glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        ));
        if (mesh->mTextureCoords[0])
        {
            uvs.push_back(glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            ));
        }
        else { uvs.push_back(glm::vec2(0.0f, 0.0f)); }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return std::async(std::launch::deferred, [&] (
        aiMesh* mesh,
        const aiScene* scene,
        std::vector<glm::vec3>&& vertices,
        std::vector<glm::vec3>&& normals,
        std::vector<glm::vec2>&& uvs,
        std::vector<GLuint>&& indices
    ) {
        EntityUID newMeshID = g_ECSManager->NewEntityUID();
        m_meshIDs.push_back(newMeshID);
        g_ECSManager->AddComponent<Transform>(newMeshID, {});  // dummy data to change later
        if (!m_material) m_material = std::make_shared<Material>(m_shader);

        if (mesh->mMaterialIndex >= 0 && m_material->getAlbedoMap() == nullptr)  // TODO: process multiple materials, only 1 material per model for now
        {
            static aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            aiString textureFilename;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilename) != -1)
            {
                Texture2D& diffuseMap = getMaterialTexture(textureFilename, aiTextureType_DIFFUSE);
                m_material->setAlbedoMap(diffuseMap);
            }
            if (material->GetTexture(aiTextureType_NORMALS, 0, &textureFilename) != -1)
            {
                Texture2D& normalMap = getMaterialTexture(textureFilename, aiTextureType_NORMALS);
                m_material->setNormalMap(normalMap);
            }
            if (material->GetTexture(aiTextureType_SPECULAR, 0, &textureFilename) != -1)
            {
                Texture2D& specularMap = getMaterialTexture(textureFilename, aiTextureType_SPECULAR);
                m_material->setSpecularMap(specularMap);
            }
            if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &textureFilename) != -1)
            {
                Texture2D& roughnessMap = getMaterialTexture(textureFilename, aiTextureType_DIFFUSE_ROUGHNESS);
                m_material->setRoughnessMap(roughnessMap);
            }
            if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &textureFilename) != -1)
            {
                Texture2D& aoMap = getMaterialTexture(textureFilename, aiTextureType_AMBIENT_OCCLUSION);
                m_material->setAoMap(aoMap);
            }
        }

        return Mesh(
            &vertices,
            &indices,
            &normals,
            nullptr,
            &uvs
        );
    }, mesh, scene, std::move(verticesPos), std::move(normals), std::move(uvs), std::move(indices));
}

Texture2D& Model::getMaterialTexture(aiString& textureFilename, aiTextureType assimp_texture_type)
{
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

    std::filesystem::path texturePath = m_modelDirectory / textureFilename.C_Str();
    FileSystem::Path project_texture_path(texturePath.string());
    
    Texture2D& texture = g_Texture2DManager.getTexture2D(project_texture_path, textureType, 1, 1);

    return texture;
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

HNCRSP_NAMESPACE_END