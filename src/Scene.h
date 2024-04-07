#pragma once

#include "src/pch/pch.h"
#include "src/ecs/ECSManager.h"
#include "src/core/ShaderManager.h"
#include "src/Renderable.h"
#include "src/Model.h"
#include "Cubemap.h"


HNCRSP_NAMESPACE_START

class Scene
{
protected:
    template <class T>
    class SceneRenderObj : public T
    {
    public:
        size_t entityUID;
        friend Scene;

    public:
        template <typename... Args>
        SceneRenderObj(Args&&... args) : T(std::forward<Args>(args)...)
        {
            entityUID = g_ECSManager->NewEntityUID();

            // default object position is at origin
            Transform newTransform;
            newTransform.position = glm::vec3(0.0f);
            g_ECSManager->AddComponent<Transform>(entityUID, newTransform);

            this->virt_AddMeshDataToRenderer(entityUID);
        }

        ~SceneRenderObj()
        {
            g_ECSManager->DestroyEntity(entityUID);
            HNCRSP_LOG_INFO("deleted");
        }

        void setShader(std::shared_ptr<Shader> newShader)
        {
            MeshData& thisMeshData = g_ECSManager->GetComponent<MeshData>(entityUID);
            thisMeshData.material->setShader(newShader);
        }

        void setTransform(const Transform& newTransform)
        {
            Transform& thisTransform = g_ECSManager->GetComponent<Transform>(entityUID);
            thisTransform = newTransform;

            if constexpr(std::is_same_v<T, Model>)
            {
                this->setAllMeshTransform(newTransform);
            }
        }
    };

private:
    std::shared_ptr<Cubemap> m_cubemap;

public:
    glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);

protected:
    template <typename TRenderable, typename... Args>
    std::unique_ptr< SceneRenderObj<TRenderable> > CreateStaticRenderObj(Args&&... args)
    {
        static_assert(std::is_base_of<Renderable, TRenderable>());

        std::unique_ptr< SceneRenderObj<TRenderable> > renderable =
            std::make_unique< SceneRenderObj<TRenderable> >(std::forward<Args>(args)...);

        return renderable;
    }

    void CreateCubemap(
        const Honeycrisp::FileSystem::Path& right,
        const Honeycrisp::FileSystem::Path& left,
        const Honeycrisp::FileSystem::Path& top,
        const Honeycrisp::FileSystem::Path& botttom,
        const Honeycrisp::FileSystem::Path& front,
        const Honeycrisp::FileSystem::Path& back
    );
    virtual void InitializeShaders(void) {}
    virtual void SetInitialUniforms(void) {}

public:
    Scene() = default;
    virtual ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&& other) noexcept = delete;
    Scene& operator=(Scene&& other) noexcept = delete;

    virtual void OnUpdate(const float& dt) = 0;
    virtual void OnImGui(void) {}
};

HNCRSP_NAMESPACE_END