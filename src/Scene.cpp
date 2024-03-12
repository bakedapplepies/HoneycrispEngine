#include "Scene.h"


HNCRSP_NAMESPACE_START

size_t Scene::m_sceneCount = 0;
std::shared_ptr<Shader> Scene::m_cubemapShader = nullptr;

Scene::Scene()
{
    m_sceneCount++;
}

Scene::~Scene()
{
    if (!m_std_moved)
    {
        m_sceneCount--;
    }
    if (m_sceneCount == 0)
    {
        m_cubemapShader.reset();
    }
    else if (m_sceneCount < 0)
    {
        HNCRSP_TERMINATE("Oops scene count is less than zero");
    }
}

Scene::Scene(Scene&& other) noexcept
{
    m_cubemap = std::move(other.m_cubemap);
    bgColor = std::move(other.bgColor);

    other.m_std_moved = true;
}

Scene& Scene::operator=(Scene&& other) noexcept
{
    m_cubemap = std::move(other.m_cubemap);
    bgColor = std::move(other.bgColor);

    other.m_std_moved = true;

    return *this;
}

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
            m_cubemapShader->setIntUnf("cubemap", 10);
        }
        m_cubemap->Draw(m_cubemapShader.get());
    }
}

HNCRSP_NAMESPACE_END