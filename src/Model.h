#pragma once

#include "pch/pch.h"
#include "Mesh.h"
#include "Debug.h"
#include "core/Texture.h"


class Model : public Renderable
{
private:
    std::vector<Mesh> m_meshes;

public:
    void Draw(std::shared_ptr<Shader> shader) const;

public:
    Model(const std::string& path, const std::source_location& location = std::source_location::current());
    Model(Model&& other) = default;
    std::filesystem::path modelDirectory;

private:
    void processNode(aiNode* node, const aiScene*);
    Mesh processMesh(aiMesh* node, const aiScene*);
    std::vector< std::shared_ptr<Texture> > loadMaterialTextures(aiMaterial* material, aiTextureType assimp_texture_type);

public:
    void OnUpdate() {}
    void AddPosition(const glm::vec3& position);
};