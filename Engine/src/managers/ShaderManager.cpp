#include "api/managers/ShaderManager.h"
#include "api/managers/SceneManager.h"
#include "src/WindowHandler.h"
#include "api/core/Timer.h"
#include "api/core/Random.h"


HNCRSP_NAMESPACE_START

ShaderManager g_ShaderManager;

static uint64_t s_key;
HNCRSP_NODISCARD uint64_t ShaderManager::StartUp()
{
    static bool startedUp = false;
    if (startedUp)
    {
        HNCRSP_ERROR("ShaderManager already initialized!");
        return UINT64_MAX;
    }
    startedUp = true;
    s_key = Random<uint64_t>::dist(0ULL, UINT64_MAX - 1);
    
    basicShader = GetShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/Default.frag")
    );
    albedoShader = GetShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/PureAlbedo.frag")
    );
    cubemapShader = GetShader(
        FileSystem::Path("resources/shaders/Cubemap.vert"),
        FileSystem::Path("resources/shaders/Cubemap.frag")
    );
    depthPassShader = GetShader(
        FileSystem::Path("resources/shaders/depth_pass/DepthPass.vert"),
        FileSystem::Path("resources/shaders/depth_pass/DepthPass.frag")
    );

    return s_key;  // key hidden by implementation
}

void ShaderManager::ShutDown(uint64_t key)
{
    if (key != s_key)
    {
        HNCRSP_TERMINATE("ShaderManager shutdown key invalid!");
        return;
    }
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
        Timer<std::chrono::milliseconds> timer(fmt::format("Fragment: {}", fragmentFile.file_name()));
        m_cachedShaders[hash_string] = std::make_unique<Shader>(vertexFile, fragmentFile, geometryFile);
        m_cachedShaders[hash_string]->SetIntUnf("u_framebuffer_depth_texture", DEPTH_BUFFER_TEXTURE_UNIT_INDEX);
        m_cachedShaders[hash_string]->SetIntUnf("u_framebuffer_depth_texture_no_alpha", DEPTH_BUFFER_NO_ALPHA_TEXTURE_UNIT_INDEX);
    }

    return m_cachedShaders[hash_string].get();
}

void ShaderManager::SetPostProcessingShader(
    const FileSystem::Path& fragmentFile  
) {
    static const CallbackData* callbackData = GetCallbackData();
    m_postprocessing_shader = GetShader(
        FileSystem::Path("resources/shaders/postprocessing/ScreenQuad.vert"),
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