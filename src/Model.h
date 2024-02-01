#pragma once

#include "src/pch/pch.h"
#include "Mesh.h"
#include "src/renderer/Texture2D.h"


HNCRSP_NAMESPACE_START

class Model : public Renderable
{
private:
    std::vector<Mesh> m_meshes;
    std::filesystem::path m_modelDirectory;

public:
    void Draw(Shader* shader) const override;

public:
    Model(const FileSystem::Path& path);
    Model(Model&& other) = default;  // TODO: Proper constructors

private:
    void processNode(aiNode* node, const aiScene*);
    Mesh processMesh(aiMesh* node, const aiScene*);
    std::vector< std::shared_ptr<Texture2D> > loadMaterialTextures(aiMaterial* material, aiTextureType assimp_texture_type);

public:
    void OnUpdate() override {}
    void addTransform(const Transform& transform) override;  // acts as interface for meshes
};

HNCRSP_NAMESPACE_END