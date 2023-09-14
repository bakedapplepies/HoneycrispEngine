#include "Model.h"


Model::Model(const std::string& path)
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

    return resultMesh;
}