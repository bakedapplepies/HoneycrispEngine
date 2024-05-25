#include "ShaderManager.h"
#include "src/managers/SceneManager.h"


HNCRSP_NAMESPACE_START

ShaderManager g_ShaderManager;

void ShaderManager::StartUp()
{
    basicShader = GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/DefaultFragment.glsl")
    );
    cubemapShader = GetShader(
        FileSystem::Path("resources/shaders/CubemapVertex.glsl"),
        FileSystem::Path("resources/shaders/CubemapFragment.glsl")
    );
}

void ShaderManager::ShutDown()
{
    m_cachedShaders.clear();
}

const Shader* ShaderManager::GetShader(
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
        m_cachedShaders[hash_string] = std::make_unique<Shader>(vertexFile, fragmentFile, geometryFile);
    }

    return m_cachedShaders[hash_string].get();

    // return std::make_shared<Shader>(vertexFile, fragmentFile, geometryFile);
}

void ShaderManager::SetPostProcessingShader(
    const FileSystem::Path& fragmentFile  
) {
    const RenderContext::CallbackData* callbackData = g_SceneManager.GetCallbackData();
    m_postprocessing_shader = GetShader(
        FileSystem::Path("resources/shaders/postprocessing/ScreenQuadVertex.glsl"),
        fragmentFile
    );
    m_postprocessing_shader->setIntUnf("u_framebuffer_color_texture", 14);
}

const Shader* ShaderManager::GetPostProcessingShader() const
{
    return m_postprocessing_shader;
}

HNCRSP_NAMESPACE_END