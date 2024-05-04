#pragma once

#include "src/pch/pch.h"
#include "src/ecs/ECSManager.h"


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
        entityUID = g_ECSManager->NewEntityUID();

        // default object position is at origin
        g_ECSManager->AddComponent<Transform>(entityUID, {});

        this->virt_AddDrawDataToRenderer(entityUID);
    }

    ~SceneRenderObj()
    {
        g_ECSManager->DestroyEntity(entityUID);
    }

    void setShader(std::shared_ptr<Shader> newShader)
    {
        DrawData& thisDrawData = g_ECSManager->GetComponent<DrawData>(entityUID);
        thisDrawData.material->setShader(newShader);
    }

    void setTransform(const Transform& newTransform)
    {
        Transform& thisTransform = g_ECSManager->GetComponent<Transform>(entityUID);
        thisTransform = newTransform;
    }
};

HNCRSP_NAMESPACE_END