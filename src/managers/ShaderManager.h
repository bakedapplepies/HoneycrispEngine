#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/graphics/core/Shader.h"


HNCRSP_NAMESPACE_START

class ShaderManager
{
private:
    std::unordered_map< std::string, std::unique_ptr<Shader> > m_cachedShaders;
    const Shader* m_postprocessing_shader = nullptr;

public:
    // TODO: make these private:
    const Shader* basicShader = nullptr;
    const Shader* albedoShader = nullptr;
    const Shader* cubemapShader = nullptr;
    const Shader* depthPassShader = nullptr;
    const Shader* depthPassNoAlphaShader = nullptr;

public:
    ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    ShaderManager(ShaderManager&&) = delete;
    ShaderManager& operator=(ShaderManager&&) = delete;
    ~ShaderManager() = default;

    void StartUp();
    void ShutDown();

    const Shader* GetShader(
        const FileSystem::Path& vertexFile,
        const FileSystem::Path& fragmentFile,
        const FileSystem::Path& geometryFile = {""}
    );
    void SetPostProcessingShader(const FileSystem::Path& fragmentFile);
    const Shader* GetPostProcessingShader() const;
};

extern ShaderManager g_ShaderManager;

HNCRSP_NAMESPACE_END