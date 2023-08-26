#include "Model.h"


Model::Model(const std::string& path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile("", aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Debug::Error("ASSIMP", import.GetErrorString());
    }
}