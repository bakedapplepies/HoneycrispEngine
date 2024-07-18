#include "ECSManager.h"
#include "src/utils/TracyProfile.h"


HNCRSP_NAMESPACE_START

ECS::ECSManager g_ECSManager;

namespace ECS
{
    HNCRSP_NODISCARD EntityUID ECSManager::NewEntityUID()
    {
        return m_entityManager.CreateEntity();
    }

    void ECSManager::DestroyEntity(EntityUID uid)
    {
        // put uid back in queue
        m_entityManager.DestroyEntity(uid);

        // delete entity's data and keep data packed
        m_componentManager.EntityDestroyed(uid);

        // delete entity uid off system
        m_systemManager.EntityDestroyed(uid);
    }

    void ECSManager::SceneCreated()
    {
        m_systemManager.SceneCreated();
    }

    void ECSManager::SceneChanged(uint32_t current_scene, uint32_t target_scene)
    {
        m_systemManager.SceneChanged(current_scene, target_scene);
    }
}  // namespace ECS

HNCRSP_NAMESPACE_END