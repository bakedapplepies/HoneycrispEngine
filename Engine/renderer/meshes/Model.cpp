#include "Model.h"

#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <fmt/format.h>

#include "core/Assert.h"
#include "util/Path.h"
#include "editor/Material.h"

HNCRSP_NAMESPACE_START

Model::Model(std::string_view path, const Envy::EnvyInstance* envy_instance)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipWindingOrder);
    bool failedResult = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;
    HNCRSP_ASSERT(!failedResult, fmt::format("Failed to load model [{}] | {}", path, importer.GetErrorString()));

    std::vector<Envy::Vertex> vertices;
    std::vector<uint32_t> indices;

    uint32_t totalNumMeshes = _FindNumMeshes(scene->mRootNode);
    m_vaoChunks.reserve(totalNumMeshes);
    m_renderCommands.reserve(totalNumMeshes);
    _RetrieveMaterials(scene, path, envy_instance);
    _ProcessNode(scene->mRootNode, scene, vertices, indices);
    m_vao = envy_instance->CreateVertexArray(vertices.data(), vertices.size(), indices.data(), indices.size());

    for (uint32_t i = 0; i < totalNumMeshes; i++)
    {
        m_renderCommands[i].vertexArray = m_vao;
    }
}

Model::~Model()
{
    m_materials.clear();
    m_renderCommands.clear();
}

std::vector<RenderCommand> Model::GetRenderCmds() const
{
    return m_renderCommands;
}

void Model::_RetrieveMaterials(const aiScene* scene, std::string_view root_path, const Envy::EnvyInstance* envy_instance)
{
    const Envy::ShaderProgram* vertexShader =
        envy_instance->GetShaderProgram(Path("engine/renderer/shaders/default.vert").Str());
    const Envy::ShaderProgram* fragmentShader =
        envy_instance->GetShaderProgram(Path("engine/renderer/shaders/default.frag").Str());
    GLResource<Envy::Pipeline> modelMatPipeline = envy_instance->CreatePipeline();
    modelMatPipeline->SetVertexProgram(vertexShader);
    modelMatPipeline->SetFragmentProgram(fragmentShader);

    m_materials.reserve(scene->mNumMaterials);
    Envy::Sampler2DConfig textureSampler2DConfig = Envy::Sampler2DConfig {
        .wrapX = GL_REPEAT,
        .wrapY = GL_REPEAT,
        .closeFilter = GL_LINEAR_MIPMAP_LINEAR,
        .farFilter = GL_NEAREST
    };
    for (uint32_t i = 0; i < scene->mNumMaterials; i++)
    {
        Material hncrspMaterial;
        hncrspMaterial.pipeline = modelMatPipeline;

        aiMaterial* material = scene->mMaterials[i];
        aiString path;
        if (aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
        {
            std::filesystem::path fullPath(root_path);
            fullPath.remove_filename();
            fullPath /= path.C_Str();
            envy_instance->LoadTexture2D(Envy::TextureFormat::SRGBA8, fullPath.string());
            hncrspMaterial.albedo = envy_instance->GetTexture2D(fullPath.string());
            hncrspMaterial.albedo->ChangeSamplerConfig(textureSampler2DConfig);
        }
        if (aiGetMaterialTexture(material, aiTextureType_NORMALS, 0, &path) == aiReturn_SUCCESS)
        {
            std::filesystem::path fullPath(root_path);
            fullPath.remove_filename();
            fullPath /= path.C_Str();
            envy_instance->LoadTexture2D(Envy::TextureFormat::SRGBA8, fullPath.string());
            hncrspMaterial.normal = envy_instance->GetTexture2D(fullPath.string());
            hncrspMaterial.normal->ChangeSamplerConfig(textureSampler2DConfig);
        }
        if (aiGetMaterialTexture(material, aiTextureType_DIFFUSE_ROUGHNESS, 0, &path) == aiReturn_SUCCESS)
        {
            std::filesystem::path fullPath(root_path);
            fullPath.remove_filename();
            fullPath /= path.C_Str();
            envy_instance->LoadTexture2D(Envy::TextureFormat::SRGBA8, fullPath.string());
            hncrspMaterial.roughness = envy_instance->GetTexture2D(fullPath.string());
            hncrspMaterial.roughness->ChangeSamplerConfig(textureSampler2DConfig);
        }

        m_materials.push_back(hncrspMaterial);
    }
}

uint32_t Model::_FindNumMeshes(const aiNode* node) const
{
    uint32_t meshes = node->mNumMeshes;
    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        meshes += _FindNumMeshes(node->mChildren[i]);
    }
    return meshes;
}

void Model::_ProcessNode(const aiNode* node,
                         const aiScene* scene,
                         std::vector<Envy::Vertex>& vertices,
                         std::vector<uint32_t>& indices)
{
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* currentMesh = scene->mMeshes[node->mMeshes[i]];
        _ProcessMesh(currentMesh, vertices, indices);
    }
    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        _ProcessNode(node->mChildren[i], scene, vertices, indices);
    }
}

void Model::_ProcessMesh(const aiMesh* mesh,
                         std::vector<Envy::Vertex>& vertices,
                         std::vector<uint32_t>& indices)
{
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 position(mesh->mVertices[i].x,
                           mesh->mVertices[i].y,
                           mesh->mVertices[i].z);
        glm::vec3 normal(mesh->mNormals[i].x,
                         mesh->mNormals[i].y,
                         mesh->mNormals[i].z);
        glm::vec2 uv;
        if (mesh->mTextureCoords[0])
        {
            uv = glm::vec2(mesh->mTextureCoords[0][i].x,
                           mesh->mTextureCoords[0][i].y);
        }
        else { uv = glm::vec2(0.0f, 0.0f); }

        vertices.push_back(Envy::Vertex {
            .position = position,
            .normal = normal,
            .uv = uv
        });

    }
    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        
        // Note: mNumIndices is guaranteed to be 3 since the model
        // was flagged to be triangulated when imported.
        // Therefore the total number of indices is mNumFaces * 3
        for (uint16_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    static int lastVertexCount = 0;
    if (m_vaoChunks.empty()) lastVertexCount = 0;  // for when loading another model

    Envy::VAOChunk vaoChunk {
        .elementsOffset = (m_vaoChunks.empty()) ? 0 : m_vaoChunks.back().elementsOffset + m_vaoChunks.back().elementsCount,
        .elementsCount = mesh->mNumFaces * 3,
        .vertexOffset = (m_vaoChunks.empty()) ? 0 : m_vaoChunks.back().vertexOffset + lastVertexCount
    };
    m_vaoChunks.push_back(vaoChunk);
    lastVertexCount = mesh->mNumVertices;

    static Transform transform;
    transform.scale = glm::vec3(0.01f);
    RenderCommand renderCmd {
        .vertexArray = m_vao,
        .vaoChunk = &m_vaoChunks.back(),  // m_vaoChunk's memory is preallocated so there
                                          // won't be dangling pointers from reallocation
        .material = &m_materials[mesh->mMaterialIndex],
        .transform = &transform
    };
    m_renderCommands.push_back(renderCmd);
}

HNCRSP_NAMESPACE_END