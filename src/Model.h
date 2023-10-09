#pragma once

#include "pch/pch.h"
#include "Mesh.h"
#include "Debug.h"
#include "core/Texture.h"


enum ETextureType
{
    DIFFUSE,
    SPECULAR
};

class Model : public Mesh
{
public:
    Model(const std::string& path, const std::source_location& location = std::source_location::current());
    std::filesystem::path modelDirectory;

private:
    std::vector<Mesh> meshes;

private:
    void processNode(aiNode* node, const aiScene*);
    Mesh processMesh(aiMesh* node, const aiScene*);
    std::vector< std::shared_ptr<Texture> > loadMaterialTextures(aiMaterial* material, aiTextureType assimp_texture_type, ETextureType texture_type, const std::string& sampler_name);
};