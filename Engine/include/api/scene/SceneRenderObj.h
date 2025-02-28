#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/ecs/ECSManager.h"
#include "api/components/Transform.h"
#include "api/scene/IRenderable.h"

HNCRSP_NAMESPACE_START

template <class T>
class SceneRenderObj : public T
{
public:
    size_t entityUID;

public:
    template <typename... Args>
    SceneRenderObj(const Material& material, Args&&... args) : T(std::forward<Args>(args)...)
    {
        entityUID = g_ECSManager.NewEntity();

        // default object position is at origin
        g_ECSManager.AddComponent<Transform>(entityUID, Transform {
            .position = glm::vec3(0.0f, 0.0f, 0.0f),
            .eulerAngles = glm::vec3(0.0f, 0.0f, 0.0f),
            .scale = glm::vec3(1.0f),
            .lookAtCamera = false
        });

        this->virt_AddDrawDataToRenderer(entityUID, material);
    }

    ~SceneRenderObj()
    {
        g_ECSManager.DestroyEntity(entityUID);
    }

    inline void SetShader(const Shader* newShader) const
    {
        DrawData& thisDrawData = g_ECSManager.GetComponent<DrawData>(entityUID);
        thisDrawData.materials[0].SetShader(newShader);
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