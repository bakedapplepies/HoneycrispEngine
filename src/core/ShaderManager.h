#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Shader.h"


HNCRSP_NAMESPACE_START

class ShaderManager
{
private:
    std::unordered_map< std::string, std::shared_ptr<Shader> > m_cachedShaders;

public:
    std::shared_ptr<Shader> basicShader;

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
};

extern ShaderManager g_ShaderManager;

HNCRSP_NAMESPACE_END