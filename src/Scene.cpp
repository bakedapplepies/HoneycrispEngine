#include "Scene.h"

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
    m_nonRenderObjectPtrs = std::move(other.m_nonRenderObjectPtrs);
    bgColor = std::move(other.bgColor);

    other.m_std_moved = true;
}

Scene& Scene::operator=(Scene&& other) noexcept
{
    m_cubemap = std::move(other.m_cubemap);
    m_renderObjectPtrs = std::move(other.m_renderObjectPtrs);
    m_nonRenderObjectPtrs = std::move(other.m_nonRenderObjectPtrs);
    bgColor = std::move(other.bgColor);

    other.m_std_moved = true;

    return *this;
}

void Scene::CreateCubemap(
    const std::string& right,
    const std::string& left,
    const std::string& top,
    const std::string& bottom,
    const std::string& front,
    const std::string& back,
    const std::source_location& location
)
{
    std::vector<std::string> cubemapFaces = {
        right, left, top, bottom, front, back
    };

    for (unsigned int i = 0; i < cubemapFaces.size(); i++)
    {  // TODO: make a ROOT macro or something for easy root folder path access
        std::string root("../../");  // Executable is in build folder
        std::filesystem::path textureRelativePath(root);
        textureRelativePath /= std::filesystem::path(location.file_name()).remove_filename();  // where the file is
        textureRelativePath /= cubemapFaces[i].c_str();  // add relative path relative to the above path <----
                                                    // in case this is absolute, it will replace everything  |
        textureRelativePath = std::filesystem::absolute(textureRelativePath);  // make absolute -------------
        textureRelativePath.make_preferred();
        cubemapFaces[i] = textureRelativePath.string();
    }

    m_cubemap = std::make_unique<Cubemap>(cubemapFaces);
}

void Scene::Draw(void) const
{
    for (auto iter = m_renderObjectPtrs.begin(); iter != m_renderObjectPtrs.end(); iter++)
    {
        iter->second.shader->Use();
        for (SceneObject<Renderable> obj : iter->second.objectShaderGroup)
        {
            obj->Draw(iter->second.shader);
        }
    }
    if (m_cubemap)
    {
        if (!m_cubemapShader)
        {
            m_cubemapShader = std::make_shared<Shader>(
                Path("../resources/shaders/cubemapvertex.glsl"),
                Path("../resources/shaders/cubemapfragment.glsl")
            );
            m_cubemapShader->setIntUniform("cubemap", 10);
        }
        m_cubemap->Draw(m_cubemapShader);
    }
}