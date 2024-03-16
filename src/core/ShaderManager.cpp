#include "ShaderManager.h"


HNCRSP_NAMESPACE_START

ShaderManager g_ShaderManager;

void ShaderManager::StartUp()
{
    basicShader = std::make_shared<Shader>(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/DefaultFragment.glsl")
    );
    cubemapShader = std::make_shared<Shader>(
        FileSystem::Path("resources/shaders/CubemapVertex.glsl"),
        FileSystem::Path("resources/shaders/CubemapFragment.glsl")
    );
}

void ShaderManager::ShutDown()
{
    m_cachedShaders.clear();
    basicShader.reset();
    cubemapShader.reset();
}

std::shared_ptr<Shader> ShaderManager::GetShader(
    const FileSystem::Path& vertexFile,
    const FileSystem::Path& fragmentFile,
    const FileSystem::Path& geometryFile
) {
    // TODO: use different hash
    std::string hash_string = vertexFile.relative_string() +
        fragmentFile.relative_string() +
        geometryFile.relative_string();

    if (m_cachedShaders[hash_string] == nullptr)
    {
        m_cachedShaders[hash_string] = std::make_shared<Shader>(vertexFile, fragmentFile, geometryFile);
    }

    return m_cachedShaders[hash_string];
}

HNCRSP_NAMESPACE_END