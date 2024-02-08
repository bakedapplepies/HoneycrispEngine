#include "Model.h"


HNCRSP_NAMESPACE_START

Model::Model(const FileSystem::Path& path)
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
    Mesh resultMesh;
    std::vector<glm::vec3> verticesPos;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indices;
    std::vector< std::shared_ptr<Texture2D> > textures;

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

    resultMesh.vertices = verticesPos;
    resultMesh.normals = normals;
    resultMesh.uvs = uvs;
    resultMesh.indices = indices;

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector< std::shared_ptr<Texture2D> > diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
        std::vector< std::shared_ptr<Texture2D> > specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
        
        textures.reserve(diffuseMaps.size() + specularMaps.size());
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    resultMesh.textures = textures;

    resultMesh.ConstructMesh();

    return resultMesh;
}

std::vector< std::shared_ptr<Texture2D> > Model::loadMaterialTextures(aiMaterial* material, aiTextureType assimp_texture_type)
{
    std::vector< std::shared_ptr<Texture2D> > textures;

    ETextureType textureType;
    switch (assimp_texture_type)
    {
    case aiTextureType_DIFFUSE:
        textureType = ETextureType::ALBEDO;
        break;
    case aiTextureType_SPECULAR:
        textureType = ETextureType::SPECULAR;
        break;
    default:
        HNCRSP_LOG_WARN("Texture2D type not recognized.");
        break;
    }

    std::unordered_map<std::string, bool> m_loadedTexturePaths;
    for (unsigned int i = 0; i < material->GetTextureCount(assimp_texture_type); i++)
    {
        aiString textureFilename;
        material->GetTexture(assimp_texture_type, i, &textureFilename);
        std::filesystem::path texturePath = m_modelDirectory / textureFilename.C_Str();
        FileSystem::Path project_texturePath(texturePath.string());
        textures.push_back(std::make_shared<Texture2D>(project_texturePath, textureType, 1, 1));
        m_loadedTexturePaths[textureFilename.C_Str()] = true;
    }
    return textures;
}

void Model::Draw(Shader* shader) const
{
    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].Draw(shader);
    }
}

void Model::addTransform(const Transform& transform)
{
    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].addTransform(transform);
    }
    transforms.push_back(transform);
};

HNCRSP_NAMESPACE_END