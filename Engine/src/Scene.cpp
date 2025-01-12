#include "api/scene/Scene.h"


HNCRSP_NAMESPACE_START

Scene::Scene(const char* scene_name)
    : m_sceneName(scene_name)
{}

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

    m_cubemap = std::make_unique<Cubemap>(cubemapFaces);
}

const Shader* Scene::CreateShader(
    const FileSystem::Path& vertex,
    const FileSystem::Path& fragment,
    const FileSystem::Path& geometry
) {
    m_shadersInScene.push_back(g_ShaderManager.GetShader(vertex, fragment, geometry));

    // notify all lights in scene to configure this shader
    for (uint32_t i = 0; i < m_currentDirectionalLights; i++)
    {
        m_lightContainer.directionalLights[i]->ConfigureShader(m_shadersInScene.back());
    }
    for (uint32_t i = 0; i < m_currentPointLights; i++)
    {
        m_lightContainer.pointLights[i]->ConfigureShader(m_shadersInScene.back());
    }
    for (uint32_t i = 0; i < m_currentSpotLights; i++)
    {
        m_lightContainer.spotLights[i]->ConfigureShader(m_shadersInScene.back());
    }

    return m_shadersInScene.back();
}

void Scene::_ReconfigureAllShaders() const
{
    for (auto& shader : m_shadersInScene)
    {
        shader->SetIntUnf("u_num_dir_light", m_currentDirectionalLights);
        shader->SetIntUnf("u_num_point_light", m_currentPointLights);
        shader->SetIntUnf("u_num_spot_light", m_currentSpotLights);
        for (uint32_t i = 0; i < m_currentDirectionalLights; i++)
        {
            m_lightContainer.directionalLights[i]->ConfigureShader(shader);
        }
        for (uint32_t i = 0; i < m_currentPointLights; i++)
        {
            m_lightContainer.pointLights[i]->ConfigureShader(shader);
        }
        for (uint32_t i = 0; i < m_currentSpotLights; i++)
        {
            m_lightContainer.spotLights[i]->ConfigureShader(shader);
        }
    }
}

HNCRSP_NAMESPACE_END