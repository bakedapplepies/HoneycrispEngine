#pragma once

#include "pch/pch.h"
#include "utils/Debug.h"
#include "Object.h"
#include "Mesh.h"
#include "NonRenderable.h"
#include "types/types_index.h"
#include "Cubemap.h"
#include "utils/utils.h"

enum EObjectRenderType
{
    NONE,
    STATIC,
    DYNAMIC
};

struct RenderableShaderGroupInfo
{
    std::vector< std::shared_ptr<Renderable> >objectShaderGroup;
    std::shared_ptr<Shader> shader;
};

class Scene
{
private:
    static size_t sceneCount;
    static std::shared_ptr<Shader> m_basicShader;
    static std::shared_ptr<Shader> m_cubemapShader;
    std::unique_ptr<Cubemap> m_cubemap;
    bool m_std_moved = false;

protected:
    std::unordered_map<GLuint, RenderableShaderGroupInfo> m_renderObjectPtrs;
    std::vector< std::shared_ptr<NonRenderable> > m_nonRenderObjectPtrs;

public:
    glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);

protected:
    template <typename T>
    SceneObject<T> CreateObject(std::true_type, T&& obj, EObjectRenderType render_type, std::shared_ptr<Shader> shader)
    {
        if (render_type == EObjectRenderType::STATIC)
        {
            SceneObject<T> objPtr = std::make_shared<T>(std::move(obj));
            // TODO: batching
            if (shader)
            {
                m_renderObjectPtrs[shader->getID()].objectShaderGroup.push_back(objPtr);
                if (!m_renderObjectPtrs[shader->getID()].shader)
                {
                    m_renderObjectPtrs[shader->getID()].shader = shader;
                }
            }
            else
            {
                m_renderObjectPtrs[m_basicShader->getID()].objectShaderGroup.push_back(objPtr);
                if (!m_renderObjectPtrs[m_basicShader->getID()].shader)
                {
                    m_renderObjectPtrs[m_basicShader->getID()].shader = m_basicShader;
                }
            }
            return objPtr;
        }
        else if (render_type == EObjectRenderType::DYNAMIC)
        {
            SceneObject<T> objPtr = std::make_shared<T>(std::move(obj));
            // TODO: instancing
            if (shader)
            {
                m_renderObjectPtrs[shader->getID()].objectShaderGroup.push_back(objPtr);
                if (!m_renderObjectPtrs[shader->getID()].shader)
                {
                    m_renderObjectPtrs[shader->getID()].shader = shader;
                }
            }
            else
            {
                m_renderObjectPtrs[m_basicShader->getID()].objectShaderGroup.push_back(objPtr);
                if (!m_renderObjectPtrs[m_basicShader->getID()].shader)
                {
                    m_renderObjectPtrs[m_basicShader->getID()].shader = m_basicShader;
                }
            }
            return objPtr;
        }
        else
        {
            // Debug::Error("EObjectRenderType enum must be either STATIC or DYNAMIC.");
            assert(false && "EObjectRenderType enum must be either STATIC or DYNAMIC.");
        }
    }

    template <typename T>
    SceneObject<T> CreateObject(std::false_type, T&& obj, EObjectRenderType render_type, std::shared_ptr<Shader> shader)
    {
        SceneObject<T> objPtr = std::make_shared<T>(std::move(obj));
        m_nonRenderObjectPtrs.push_back(objPtr);
        return objPtr;
    }

    template <typename T>
    SceneObject<T> CreateObject(T&& obj, EObjectRenderType render_type = EObjectRenderType::NONE, std::shared_ptr<Shader> shader = nullptr)
    {
        if (!m_basicShader)
        {
            m_basicShader = std::make_shared<Shader>(
                FileSystem::Path("../resources/shaders/DefaultVertex.glsl"),
                FileSystem::Path("../resources/shaders/DefaultFragment.glsl")
            );
        }
        
        return CreateObject(std::is_base_of<Renderable, T>(), std::move(obj), render_type, shader);
    }

    void CreateCubemap(
        const std::string& right,
        const std::string& left,
        const std::string& top,
        const std::string& botttom,
        const std::string& front,
        const std::string& back,
        const std::source_location& location = std::source_location::current()
    );
    void Draw(void) const;

public:
    Scene();
    virtual ~Scene()
    {
        m_renderObjectPtrs.clear();
        if (!m_std_moved)
        {
            sceneCount--;
        }
        if (sceneCount == 0)
        {
            m_basicShader.reset();
            m_cubemapShader.reset();
        }
        else if (sceneCount < 0)
        {
            Debug::Error("Oops shader count is less than zero");
            assert(false);
        }
    }
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&& other) noexcept;
    Scene& operator=(Scene&& other) noexcept;

    virtual void OnUpdate() = 0;  // force a derived method
    virtual void InitializeShaders(void) {}
    virtual void SetInitialUniforms(void) {}
};