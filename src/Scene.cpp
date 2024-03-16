#include "Scene.h"


HNCRSP_NAMESPACE_START

std::shared_ptr<Shader> Scene::m_cubemapShader = nullptr;

void Scene::CreateCubemap(
    const FileSystem::Path& right,
    const FileSystem::Path& left,
    const FileSystem::Path& top,
    const FileSystem::Path& bottom,
    const FileSystem::Path& front,
    const FileSystem::Path& back
)
{
    std::vector<std::string> cubemapFaces = {
        right.string(),
        left.string(),
        top.string(),
        bottom.string(),
        front.string(),
        back.string()
    };

    m_cubemap = std::make_unique<Cubemap>(cubemapFaces);
}

void Scene::DrawCubemap(void) const
{
    if (m_cubemap)
    {
        if (!m_cubemapShader)
        {
            m_cubemapShader = std::make_shared<Shader>(
                FileSystem::Path("resources/shaders/CubemapVertex.glsl"),
                FileSystem::Path("resources/shaders/CubemapFragment.glsl")
            );
            m_cubemapShader->Use();
            m_cubemapShader->setIntUnf("cubemap", 10);
        }
        m_cubemap->Draw(m_cubemapShader.get());
    }
}

HNCRSP_NAMESPACE_END