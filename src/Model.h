#pragma once

#include "pch/pch.h"
#include "Mesh.h"
#include "utils/Debug.h"
#include "utils/utils.h"
#include "core/Texture.h"


class Model : public Renderable
{
private:
    std::vector<Mesh> m_meshes;
    std::filesystem::path m_modelDirectory;

public:
    void Draw(std::shared_ptr<Shader> shader) const;

public:
    Model(const FileSystem::Path& path);
    Model(Model&& other) = default;  // TODO: Proper constructors

private:
    void processNode(aiNode* node, const aiScene*);
    Mesh processMesh(aiMesh* node, const aiScene*);
    std::vector< std::shared_ptr<Texture2D> > loadMaterialTextures(aiMaterial* material, aiTextureType assimp_texture_type);

public:
    void onUpdate() {}
    void addTransform(const Transform& transform);  // acts as interface for meshes
};