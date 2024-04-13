#pragma once

#include "src/pch/pch.h"
#include "Mesh.h"
#include "src/serialize/ModelSerializer.h"


HNCRSP_NAMESPACE_START

class Model : public Renderable
{
private:
    std::vector<Mesh> m_meshes;
    std::vector<EntityUID> m_meshIDs;

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Material> m_material;

public:
    Model(const FileSystem::Path& path, std::shared_ptr<Shader> shader, bool flip_uv);
    Model(Model&& other) = default;  // TODO: Proper constructors

    void setAllMeshTransform(const Transform& transform) const;
    void virt_AddMeshDataToRenderer(EntityUID entityUID, std::shared_ptr<Material> material = nullptr) override final;
    Material* getMaterial() const;

private:  // building model
    void processNode(
        aiNode* node,
        const aiScene*,
        const FileSystem::Path& modelDirectory,
        ModelSerializer& modelSerializer
    );
    Mesh processMesh(
        aiMesh* node,
        const aiScene*,
        const FileSystem::Path& modelDirectory,
        ModelSerializer& modelSerializer
    );
    std::shared_ptr<Texture2D> getMaterialTexture(
        std::string_view texturePath,
        aiTextureType assimp_texture_type
    );
    void loadDeserializedModel(
        const Serialized::Model* deserialized_model,
        std::shared_ptr<Shader> shader    
    );
};

HNCRSP_NAMESPACE_END