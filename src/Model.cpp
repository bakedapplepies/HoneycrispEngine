#include "Model.h"
#include "src/core/Texture2DManager.h"


HNCRSP_NAMESPACE_START

Model::Model(const FileSystem::Path& path, std::shared_ptr<Shader> shader)
    : m_shader(shader)
{
    // Model loading
    float beginTime = glfwGetTime();
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path.string().data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HNCRSP_TERMINATE(fmt::format("ASSIMP: {}", import.GetErrorString()).c_str());
    }
    HNCRSP_LOG_INFO(  // display relative path from project directory
        fmt::format(
            "Model load time ~{}: {}s",
            std::filesystem::relative(path.string(), HNCRSP_PROJECT_DIR).string(),
            glfwGetTime() - beginTime
        )
    );
    m_modelDirectory = std::filesystem::path(path.string());
    m_modelDirectory = m_modelDirectory.remove_filename();

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    EntityUID newMeshID = g_ECSManager->NewEntityUID();
    m_meshIDs.push_back(newMeshID);

    g_ECSManager->AddComponent<Transform>(newMeshID, {});  // dummy data to change later

    m_material = std::make_shared<Material>(m_shader);

    std::vector<glm::vec3> verticesPos;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indices;

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
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh resultMesh(
        &verticesPos,
        &indices,
        &normals,
        nullptr,
        &uvs
    );

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        Texture2D& diffuseMap = getMaterialTexture(material, aiTextureType_DIFFUSE);
        Texture2D& specularMap = getMaterialTexture(material, aiTextureType_SPECULAR);

        m_material->setAlbedoMap(diffuseMap);
        m_material->setSpecularMap(specularMap);
    }

    return resultMesh;
}

Texture2D& Model::getMaterialTexture(aiMaterial* material, aiTextureType assimp_texture_type)
{
    ETextureType textureType = ETextureType::NONE;
    switch (assimp_texture_type)
    {
    case aiTextureType_DIFFUSE:
        textureType = ETextureType::ALBEDO;
        break;

    case aiTextureType_SPECULAR:
        textureType = ETextureType::SPECULAR;
        break;

    default:
        HNCRSP_TERMINATE("Texture2D type not recognized.");
        break;
    }

    aiString textureFilename;
    material->GetTexture(assimp_texture_type, 0, &textureFilename);
    std::filesystem::path texturePath = m_modelDirectory / textureFilename.C_Str();
    FileSystem::Path project_texture_path(texturePath.string());
    
    Texture2D& texture = g_Texture2DManager.getTexture2D(project_texture_path, textureType, 1, 1);

    return texture;
}

void Model::setAllMeshTransform(const Transform& transform)
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
        m_meshes[i].virt_AddMeshDataToRenderer(m_meshIDs[i], material);
    }
}

HNCRSP_NAMESPACE_END