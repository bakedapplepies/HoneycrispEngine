#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Shader.h"


HNCRSP_NAMESPACE_START

class ShaderManager
{
private:
    std::unordered_map< std::string, std::shared_ptr<Shader> > m_cachedShaders;
    std::shared_ptr<Shader> m_postprocessing_shader;

public:
    // TODO: make these private:
    std::shared_ptr<Shader> basicShader;
    std::shared_ptr<Shader> cubemapShader;

public:
    ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    ShaderManager(ShaderManager&&) = delete;
    ShaderManager& operator=(ShaderManager&&) = delete;
    ~ShaderManager() = default;

    void StartUp();
    void ShutDown();

    std::shared_ptr<Shader> GetShader(
        const FileSystem::Path& vertexFile,
        const FileSystem::Path& fragmentFile,
        const FileSystem::Path& geometryFile = {""}
    );
    void SetPostProcessingShader(const FileSystem::Path& fragmentFile);
    std::shared_ptr<Shader> GetPostProcessingShader() const;
};

extern ShaderManager g_ShaderManager;

HNCRSP_NAMESPACE_END