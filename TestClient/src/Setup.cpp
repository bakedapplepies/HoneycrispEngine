#include "Setup.h"
#include "api/managers/ShaderManager.h"

using namespace Honeycrisp;

void PreloadShaders()
{
    // Basic shader
    g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/Default.frag")
    );
    // Albedo shader
    g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/PureAlbedo.frag")
    );
    // Cubemap shader
    g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/Cubemap.vert"),
        FileSystem::Path("resources/shaders/Cubemap.frag")
    );
    // Depth-pass shader
    g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/depth_pass/DepthPass.vert"),
        FileSystem::Path("resources/shaders/depth_pass/DepthPass.frag")
    );
    // Phong shader
    g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/BlinnPhong.frag")
    );
    // Phong (w/ tint) shader
    g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/Default.vert"),
        FileSystem::Path("resources/shaders/BlinnPhongTint.frag")
    );
}