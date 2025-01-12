#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/Shader.h"

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

public:
    ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    ShaderManager(ShaderManager&&) = delete;
    ShaderManager& operator=(ShaderManager&&) = delete;
    ~ShaderManager() = default;

    HNCRSP_NODISCARD uint64_t StartUp();
    void ShutDown(uint64_t key);

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