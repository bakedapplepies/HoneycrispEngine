#pragma once

#include "src/pch/pch.h"
#include "Mesh.h"
#include "src/opengl/Texture2D.h"


HNCRSP_NAMESPACE_START

class Model : public Renderable
{
private:
    std::vector<Mesh> m_meshes;
    std::vector<EntityUID> m_meshIDs;
    std::filesystem::path m_modelDirectory;

public:
    Model(const FileSystem::Path& path);
    Model(Model&& other) = default;  // TODO: Proper constructors

public:
    void Draw(Shader* shader) const;
    void setAllMeshTransform(const Transform& transform);

    void virt_AddMeshDataToRenderer(EntityUID entityUID) override final;

private:
    void processNode(aiNode* node, const aiScene*);
    Mesh processMesh(aiMesh* node, const aiScene*);
    Texture2D& getMaterialTexture(aiMaterial* material, aiTextureType assimp_texture_type);
};

HNCRSP_NAMESPACE_END