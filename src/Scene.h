#pragma once

#include "src/pch/pch.h"

#include "Renderable.h"
#include "Cubemap.h"
#include "utils/utils.h"


HNCRSP_NAMESPACE_START

class Scene  // : public std::enable_shared_from_this<Scene>
{
protected:
    // TODO: SceneObject is really just a Renderable, redesign this, maybe switch to just a shared/unique_ptr
    template <class T>
    class SceneObject : public T, public std::enable_shared_from_this<SceneObject<T>>
    {
    private:  // TODO: Check if we really need a pointer the the parent class
        Scene* scene;
        GLuint shaderID;

    public:
        size_t sceneObjectID;

    public:
        template <typename... Args>
        SceneObject(Scene* scene, Args&&... args) : T(std::forward<T>(args)...), scene(scene)
        {
            sceneObjectID = scene->genSceneObjectID();
            shaderID = scene->m_basicShader->getID();
        }

    public:  // TODO: private everything & befriend parent class, only expose class type to the outside
        void setShader(std::shared_ptr<Shader> newShader)
        {
            Scene::binary_delete_ptr(
                scene->m_renderObjectPtrs[shaderID].objectShaderGroup,
                sceneObjectID
            );
            shaderID = newShader->getID();
            if (!scene->m_renderObjectPtrs[shaderID].shader)
            {
                scene->m_renderObjectPtrs[shaderID].shader = newShader;
            }
            Scene::binary_insert_ptr(
                scene->m_renderObjectPtrs[shaderID].objectShaderGroup,
                { sceneObjectID, this->shared_from_this() }
            );
        }
    };
    
    enum EObjectMovement
    {
        NONE,
        DYNAMIC
    };

private:
    struct RenderableShaderGroupInfo
    {
        std::vector< std::pair< size_t, std::shared_ptr<Renderable> > > objectShaderGroup;
        std::shared_ptr<Shader> shader;
    };

private:
    // Make sure vector is already sorted
    static void binary_insert_ptr(
        std::vector<std::pair<size_t, std::shared_ptr<Renderable>>>& vec,
        const std::pair<size_t, std::shared_ptr<Renderable>>& pair
    );

    static void binary_delete_ptr(std::vector<std::pair<size_t, std::shared_ptr<Renderable>>>& vec, const size_t& objID);

private:
    static size_t sceneCount;
    static std::shared_ptr<Shader> m_basicShader;
    static std::shared_ptr<Shader> m_cubemapShader;
    std::unique_ptr<Cubemap> m_cubemap;
    bool m_std_moved = false;

    size_t nextSceneObjectID = 0;
    std::priority_queue< size_t, std::vector<size_t>, std::greater<size_t> > sceneObjectIDQueue;

protected:
    std::unordered_map<GLuint, RenderableShaderGroupInfo> m_renderObjectPtrs;
    std::vector< std::shared_ptr<Object> > m_nonRenderObjectPtrs;

public:
    glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);

private:
    template <typename T, typename... Args>
    std::shared_ptr< SceneObject<T> > CreateMovable(std::true_type, Args&&... args)
    {  // SceneObject<T> is still of type Renderable, can be added normally to m_renderObjectPtrs
        std::shared_ptr< SceneObject<T> > objPtr = std::make_shared< SceneObject<T> >(this, std::forward<Args>(args)...);
        
        // TODO: batching
        Scene::binary_insert_ptr(
            m_renderObjectPtrs[m_basicShader->getID()].objectShaderGroup,
            { objPtr->sceneObjectID, objPtr }
        );
        if (!m_renderObjectPtrs[m_basicShader->getID()].shader)
        {
            m_renderObjectPtrs[m_basicShader->getID()].shader = m_basicShader;
        }
        return objPtr;
    }

    template <typename T, typename... Args>
    std::shared_ptr< SceneObject<T> > CreateImmovable(std::true_type, Args&&... args)
    {
        std::shared_ptr< SceneObject<T> > objPtr = std::make_shared< SceneObject<T> >(this, std::forward<Args>(args)...);
        
        // TODO: instancing
        Scene::binary_insert_ptr(
            m_renderObjectPtrs[m_basicShader->getID()].objectShaderGroup,
            { objPtr->sceneObjectID, objPtr }
        );
        if (!m_renderObjectPtrs[m_basicShader->getID()].shader)
        {
            m_renderObjectPtrs[m_basicShader->getID()].shader = m_basicShader;
        }
        return objPtr;
    }

    template <typename T, typename... Args>
    std::shared_ptr< SceneObject<T> > CreateMovable(std::false_type, Args&&... args)
    {
        std::shared_ptr< SceneObject<T> > objPtr = std::make_shared< SceneObject<T> >(this, std::forward<Args>(args)...);
        m_nonRenderObjectPtrs.push_back(objPtr);
        return objPtr;
    }

    template <typename T, typename... Args>
    std::shared_ptr< SceneObject<T> > CreateImmovable(std::false_type, Args&&... args)
    {
        std::shared_ptr< SceneObject<T> > objPtr = std::make_shared< SceneObject<T> >(this, std::forward<Args>(args)...);
        m_nonRenderObjectPtrs.push_back(objPtr);
        return objPtr;
    }

protected:
    template <typename T, EObjectMovement movement_type, typename... Args>
    std::shared_ptr< SceneObject<T> > CreateObject(Args&&... args)
    {
        static_assert(std::is_base_of<Object, T>());
        
        using namespace Honeycrisp::FileSystem;
        if (!m_basicShader)  // just initializing basic shader
        {
            m_basicShader = std::make_shared<Shader>(
                Path("resources/shaders/DefaultVertex.glsl"),
                Path("resources/shaders/DefaultFragment.glsl")
            );
        }
        
        // TODO: use forward to check for regular refs
        if (movement_type == EObjectMovement::DYNAMIC)
        {
            return CreateMovable<T>(std::is_base_of<Renderable, T>(), std::forward<Args>(args)...);
        }
        else
        {
            return CreateImmovable<T>(std::is_base_of<Renderable, T>(), std::forward<Args>(args)...);
        }
    }

    void CreateCubemap(
        const Honeycrisp::FileSystem::Path& right,
        const Honeycrisp::FileSystem::Path& left,
        const Honeycrisp::FileSystem::Path& top,
        const Honeycrisp::FileSystem::Path& botttom,
        const Honeycrisp::FileSystem::Path& front,
        const Honeycrisp::FileSystem::Path& back
    );
    void Draw(void) const;
    virtual void InitializeShaders(void) {}
    virtual void SetInitialUniforms(void) {}

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
            HNCRSP_TERMINATE("Oops scene count is less than zero");
        }
    }
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&& other) noexcept;
    Scene& operator=(Scene&& other) noexcept;

    virtual void OnUpdate() = 0;  // force overload
    virtual void OnImGui(void) {}

    virtual size_t genSceneObjectID() final;
    virtual void deleteSceneObjectID(size_t id) final;
};

HNCRSP_NAMESPACE_END