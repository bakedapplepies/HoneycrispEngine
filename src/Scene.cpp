#include "Scene.h"


HNCRSP_NAMESPACE_START

size_t Scene::sceneCount = 0;
std::shared_ptr<Shader> Scene::m_basicShader = nullptr;
std::shared_ptr<Shader> Scene::m_cubemapShader = nullptr;

Scene::Scene()
{
    sceneCount++;
}

Scene::Scene(Scene&& other) noexcept
{
    m_cubemap = std::move(other.m_cubemap);
    m_renderObjectPtrs = std::move(other.m_renderObjectPtrs);
    bgColor = std::move(other.bgColor);

    other.m_std_moved = true;
}

Scene& Scene::operator=(Scene&& other) noexcept
{
    m_cubemap = std::move(other.m_cubemap);
    m_renderObjectPtrs = std::move(other.m_renderObjectPtrs);
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

void Scene::Draw(void) const
{
    for (auto iter = m_renderObjectPtrs.begin(); iter != m_renderObjectPtrs.end(); iter++)
    {
        iter->second.shader->Use();
        for (std::pair< size_t, std::shared_ptr<Mesh> > obj : iter->second.objectShaderGroup)
        {
            obj.second->Draw(iter->second.shader.get());
        }
    }
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

size_t Scene::genSceneObjectID()
{
    if (!sceneObjectIDQueue.empty())
    {
        size_t id = sceneObjectIDQueue.top();
        sceneObjectIDQueue.pop();
        return id;
    }
    
    return nextSceneObjectID++;
}

void Scene::deleteSceneObjectID(size_t id)
{
    if (id == nextSceneObjectID - 1)  // id of last created SceneObject
    {
        nextSceneObjectID--;
    }
    else
    {
        sceneObjectIDQueue.push(id);
    }
}

// To make sure vector is already sorted
// template <template<typename> typename T, typename U>
void Scene::binary_insert_ptr(
    std::vector<std::pair<size_t, std::shared_ptr<Mesh>>>& vec, const std::pair<size_t, std::shared_ptr<Mesh>>& pair)
{
    // static_assert(std::is_arithmetic_v<T>, "Type T is not of arithmetic type");

    if (vec.size() == 0)
    {
        vec.push_back({ pair.first, pair.second });
        return;
    }
    else if (vec.size() == 1)
    {
        size_t pos = static_cast<size_t>(pair.first > vec[0].first);
        vec.insert(vec.begin() + pos, { pair.first, pair.second });
    }

    size_t left = 0, right = vec.size() - 1;
    size_t mid = (left + right) / 2;

    while (!(vec[mid].first < pair.first && pair.first < vec[mid + 1].first))
    {
        if (pair.first < vec[mid].first) right = mid;
        else left = mid + 1;

        mid = (left + right) / 2;

        if (mid == vec.size() - 1) break;
        else if (mid == 0) break;
    }

    vec.insert(vec.begin() + mid + (size_t)(mid != 0), { pair.first, pair.second });
}

// template <typename T>
void Scene::binary_delete_ptr(std::vector<std::pair<size_t, std::shared_ptr<Mesh>>>& vec, const size_t& objID)
{
    // static_assert(std::is_arithmetic_v<T>, "Type T is not of arithmetic type");

    if (vec.size() == 0)
    {
        HNCRSP_TERMINATE("Vector is empty");
    }

    size_t left = 0, right = vec.size() - 1;
    size_t mid = (left + right) / 2;

    while (objID != vec[mid].first)
    {
        if (objID < vec[mid].first) right = mid - 1;
        else left = mid + 1;

        mid = (left + right) / 2;
    }

    vec.erase(vec.begin() + mid);
}

HNCRSP_NAMESPACE_END