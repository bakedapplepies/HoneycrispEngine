#include "ShaderManager.h"
#include "src/managers/SceneManager.h"
#include "src/utils/Timer.h"


HNCRSP_NAMESPACE_START

ShaderManager g_ShaderManager;

void ShaderManager::StartUp()
{
    basicShader = GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/DefaultFragment.glsl")
    );
    albedoShader = GetShader(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/PureAlbedoFragment.glsl")
    );
    cubemapShader = GetShader(
        FileSystem::Path("resources/shaders/CubemapVertex.glsl"),
        FileSystem::Path("resources/shaders/CubemapFragment.glsl")
    );
    depthPassShader = GetShader(
        FileSystem::Path("resources/shaders/depth_pass/DepthPassVertex.glsl"),
        FileSystem::Path("resources/shaders/depth_pass/DepthPassFragment.glsl")
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
        Timer<std::chrono::milliseconds> timer("Shader creation");
        m_cachedShaders[hash_string] = std::make_unique<Shader>(vertexFile, fragmentFile, geometryFile);
        m_cachedShaders[hash_string]->SetIntUnf("u_framebuffer_depth_texture", DEPTH_BUFFER_TEXTURE_UNIT_INDEX);
        m_cachedShaders[hash_string]->SetIntUnf("u_framebuffer_depth_texture_no_alpha", DEPTH_BUFFER_NO_ALPHA_TEXTURE_UNIT_INDEX);
    }

    return m_cachedShaders[hash_string].get();
}

void ShaderManager::SetPostProcessingShader(
    const FileSystem::Path& fragmentFile  
) {
    const CallbackData* callbackData = g_SceneManager.GetCallbackData();
    m_postprocessing_shader = GetShader(
        FileSystem::Path("resources/shaders/postprocessing/ScreenQuadVertex.glsl"),
        fragmentFile
    );
    m_postprocessing_shader->SetIntUnf("u_framebuffer_color_texture", COLOR_BUFFER_TEXTURE_UNIT_INDEX);
    m_postprocessing_shader->SetVec2Unf("u_viewport_size", glm::vec2(callbackData->windowWidth * 0.8f, callbackData->windowHeight));
}

const Shader* ShaderManager::GetPostProcessingShader() const
{
    return m_postprocessing_shader;
}

HNCRSP_NAMESPACE_END