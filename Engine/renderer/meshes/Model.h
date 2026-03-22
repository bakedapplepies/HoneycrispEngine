#pragma once

#include <Envy/EnvyInstance.h>
#include <assimp/scene.h>

#include "core/Defines.h"
#include "core/Types.h"
#include "editor/Material.h"
#include "renderer/Renderer.h"

HNCRSP_NAMESPACE_START

class Model
{
public:
    Model(std::string_view path, const Envy::EnvyInstance* envy_instance);
    ~Model();

    std::vector<RenderCommand> GetRenderCmds() const;

private:
    void _RetrieveMaterials(const aiScene* scene, std::string_view root_path, const Envy::EnvyInstance* envy_instance);
    uint32_t _FindNumMeshes(const aiNode* node) const;
    void _ProcessNode(const aiNode* node,
                      const aiScene* scene,
                      std::vector<Envy::Vertex>& vertices,
                      std::vector<uint32_t>& indices);
    void _ProcessMesh(const aiMesh* mesh,
                      std::vector<Envy::Vertex>& vertices,
                      std::vector<uint32_t>& indices);

private:
    // TODO: Make this an std::array (kinda like RayLib)
    std::vector<Material> m_materials;
    GLResource<Envy::VertexArray> m_vao;
    std::vector<Envy::VAOChunk> m_vaoChunks;
    std::vector<RenderCommand> m_renderCommands;
};

HNCRSP_NAMESPACE_END