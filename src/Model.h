#pragma once

#include "src/pch/pch.h"
#include "Mesh.h"
#include "src/utils/Command.h"


HNCRSP_NAMESPACE_START

class Model : public Renderable
{
private:
    std::filesystem::path m_modelDirectory;
    std::vector<Mesh> m_meshes;
    std::vector<EntityUID> m_meshIDs;

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Material> m_material;

public:
    Model(const FileSystem::Path& path, std::shared_ptr<Shader> shader, bool flip_uv);
    Model(Model&& other) = default;  // TODO: Proper constructors

    void Draw(Shader* shader) const;
    void setAllMeshTransform(const Transform& transform) const;
    void virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material = nullptr) override final;

private:
    void processNode(aiNode* node, const aiScene*, std::vector< std::future< std::future<Mesh> > >& future_meshes);
    std::future<Mesh> processMesh(aiMesh* node, const aiScene*);
    Texture2D& getMaterialTexture(aiString& textureFilename, aiTextureType assimp_texture_type);
};

HNCRSP_NAMESPACE_END