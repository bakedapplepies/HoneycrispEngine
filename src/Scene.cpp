#include "Scene.h"


HNCRSP_NAMESPACE_START

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
        m_cubemap->Draw();
    }
}

HNCRSP_NAMESPACE_END