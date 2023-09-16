#pragma once

#include "pch/pch.h"
#include "Mesh.h"
#include "Debug.h"
#include "Texture.h"


enum ETextureType {
    DIFFUSE,
    SPECULAR
};

class Model : public Mesh
{
public:
    Model(const std::string& path);
    std::string modelDirectory;

private:
    std::vector<Mesh> meshes;

private:
    void processNode(aiNode* node, const aiScene*);
    Mesh processMesh(aiMesh* node, const aiScene*);
    std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType assimp_texture_type, ETextureType texture_type);
};