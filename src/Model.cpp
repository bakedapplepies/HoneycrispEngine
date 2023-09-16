#include "Model.h"


Model::Model(const std::string& path) : modelDirectory(path)
{
    float beginTime = glfwGetTime();
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Debug::Error("ASSIMP: ", import.GetErrorString());
    }   
    Debug::Log(fmt::format("Time took to load {}: {}s", path, glfwGetTime() - beginTime));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
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
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        verticesPos.push_back(glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].x,
            mesh->mVertices[i].z
        ));
        normals.push_back(glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].x,
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
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, ETextureType::DIFFUSE);
    }

    return resultMesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType assimp_texture_type, ETextureType texture_type)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(assimp_texture_type); i++)
    {
        aiString str;
        material->GetTexture(assimp_texture_type, i, &str);
        Texture texture;

    }
}