#include "Scene.h"


HNCRSP_NAMESPACE_START

void Scene::CreateCubemap(
    const FileSystem::Path& right,
    const FileSystem::Path& left,
    const FileSystem::Path& top,
    const FileSystem::Path& bottom,
    const FileSystem::Path& front,
    const FileSystem::Path& back
) {
    std::array<FileSystem::Path, 6> cubemapFaces = {
        right, left, top, bottom, front, back
    };

    cubemap = std::make_unique<Cubemap>(cubemapFaces);
}

const Shader* Scene::CreateShader(
    const FileSystem::Path& vertex,
    const FileSystem::Path& fragment,
    const FileSystem::Path& geometry
) {
    m_shadersInScene.push_back(g_ShaderManager.GetShader(vertex, fragment, geometry));

    // notify all lights in scene to configure this shader
    for (auto& light : m_lightsInscene)
    {
        light->ConfigureShader(m_shadersInScene.back());
    }

    return m_shadersInScene.back();
}

void Scene::_ReconfigureAllShaders() const
{
    for (auto& light : m_lightsInscene)
    {
        for (auto& shader : m_shadersInScene)
        {
            light->ConfigureShader(shader);
        }
    }
}

HNCRSP_NAMESPACE_END