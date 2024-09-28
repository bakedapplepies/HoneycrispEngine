#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/ecs/ECSManager.h"
#include "src/Renderable.h"


HNCRSP_NAMESPACE_START

template <class T>
class SceneRenderObj : public T
{
public:
    size_t entityUID;

public:
    template <typename... Args>
    SceneRenderObj(Args&&... args) : T(std::forward<Args>(args)...)
    {
        static_assert(std::is_base_of<Renderable, T>());
        entityUID = g_ECSManager.NewEntityUID();

        // default object position is at origin
        g_ECSManager.AddComponent<Transform>(entityUID, {});

        this->virt_AddDrawDataToRenderer(entityUID);
    }

    ~SceneRenderObj()
    {
        g_ECSManager.DestroyEntity(entityUID);
    }

    inline void SetShader(const Shader* newShader) const
    {
        DrawData& thisDrawData = g_ECSManager.GetComponent<DrawData>(entityUID);
        thisDrawData.materials[0]->SetShader(newShader);
    }

    inline void SetTransform(const Transform& newTransform) const
    {
        g_ECSManager.GetComponent<Transform>(entityUID) = newTransform;
    }

    inline void SetPosition(const glm::vec3& position) const
    {
        g_ECSManager.GetComponent<Transform>(entityUID).position = position;
    }
};

HNCRSP_NAMESPACE_END