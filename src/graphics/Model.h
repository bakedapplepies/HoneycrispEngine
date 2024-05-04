#pragma once

#include "src/pch/pch.h"
#include "src/Renderable.h"
#include "src/serialize/ModelSerializer.h"
#include "src/components/DrawData.h"


HNCRSP_NAMESPACE_START

class Scene;

class Model : public Renderable
{
private:
    std::unique_ptr<VertexArray> m_VAO;
    std::vector<MeshMetaData> m_meshesMetaData;  // size is also number of meshes
    std::shared_ptr<Material> m_material;

public:
    Model(const FileSystem::Path& path, std::shared_ptr<Shader> shader, bool flip_uv);
    Model(Model&& other) = default;  // TODO: Proper constructors
    Material* getMaterial() const;
    void virt_AddDrawDataToRenderer(EntityUID entityUID) override final;

private:  // building model
    void processNode(
        aiNode* node,
        const aiScene*,
        const FileSystem::Path& modelDirectory,
        ModelSerializer& modelSerializer,
        std::vector<float>& vertexData,
        std::vector<GLuint>& indices
    );
    void processMesh(
        aiMesh* node,
        const aiScene*,
        const FileSystem::Path& modelDirectory,
        ModelSerializer& modelSerializer,
        std::vector<float>& vertexData,
        std::vector<GLuint>& indices
    );
    std::shared_ptr<Texture2D> getMaterialTexture(
        std::string_view texturePath,
        aiTextureType assimp_texture_type
    );
    void loadDeserializedModel(const Serialized::Model* deserialized_model);
};

HNCRSP_NAMESPACE_END