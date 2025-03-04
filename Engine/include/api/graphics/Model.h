#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/scene/IRenderable.h"
#include "api/serialization/ModelSerializer.h"
#include "api/components/DrawData.h"

HNCRSP_NAMESPACE_START

class Scene;

class Model : public IRenderable
{
private:
    VertexArray m_VAO;
    // TODO: Deallocate these
    std::vector<MeshMetaData> m_meshesMetaData;  // size is also number of meshes
    std::vector<Material> m_materials;
    const Shader* m_shader;

public:
    Model(const FileSystem::Path& path, const Shader* shader, bool flip_uv);
    Model(const Model&) = delete;
    Model(Model&& other) noexcept;  // TODO: Proper constructors
    Model& operator=(const Model&) = delete;
    Model& operator=(Model&& other) noexcept;
    void virt_AddDrawDataToRenderer(
        ECS::EntityUID entityUID,
        HNCRSP_MAYBE_UNUSED const Material& material
    ) const override final;

private:  // building model
    void _ProcessNode(
        aiNode* node,
        const aiScene*,
        const FileSystem::Path& modelDirectory,
        ModelSerializer& modelSerializer,
        std::vector<float>& vertexData,
        std::vector<GLuint>& indices
    );
    void _ProcessMesh(
        aiMesh* node,
        const aiScene*,
        const FileSystem::Path& modelDirectory,
        ModelSerializer& modelSerializer,
        std::vector<float>& vertexData,
        std::vector<GLuint>& indices
    );
    void _GetMaterials(
        const aiScene* scene,
        const FileSystem::Path& modelDirectory,
        ModelSerializer& modelSerializer
    );
    const Texture2D* _GetMaterialTexture(
        std::string_view texturePath,
        aiTextureType assimp_texture_type
    );
    void _LoadDeserializedModel(const Serialized::Model* deserialized_model);
};

HNCRSP_NAMESPACE_END