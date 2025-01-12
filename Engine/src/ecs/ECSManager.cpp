#include "api/ecs/ECSManager.h"
#include "api/core/TracyProfile.h"
#include "api/core/Random.h"


HNCRSP_NAMESPACE_START

ECS::ECSManager g_ECSManager;

namespace ECS
{
    static uint64_t s_key;
    HNCRSP_NODISCARD uint64_t ECSManager::StartUp()
    {
        static bool startedUp = false;
        if (startedUp)
        {
            HNCRSP_ERROR("ECSManager already initialized!");
            return UINT64_MAX;
        }
        startedUp = true;
        s_key = Random<uint64_t>::dist(0ULL, UINT64_MAX - 1);

        m_entityManager.StartUp();
        m_componentManager.StartUp();
        m_systemManager.StartUp();

        return s_key;  // key hidden by implementation
    }

    void ECSManager::ShutDown(uint64_t key)
    {
        if (key != s_key)
        {
            HNCRSP_ERROR("ECSManager shutdown key invalid!");
            return;
        }
        m_entityManager.ShutDown();
        m_componentManager.ShutDown();
        m_systemManager.ShutDown();
    }

    EntityUID ECSManager::NewEntity()
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

    void ECSManager::_SceneCreated()
    {
        m_systemManager.SceneCreated();
    }

    void ECSManager::_SceneChanged(uint32_t current_scene, uint32_t target_scene)
    {
        m_systemManager.SceneChanged(current_scene, target_scene);
    }
}  // namespace ECS

HNCRSP_NAMESPACE_END