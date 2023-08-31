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
}