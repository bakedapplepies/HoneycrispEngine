#include "EntityManager.h"
#include <cassert>


HNCRSP_NAMESPACE_START

namespace ECS
{
    void EntityManager::StartUp()
    {
        for (EntityUID uid = 0; uid < MAX_ENTITIES; uid++)
        {
            m_availableEntityUIDs.push(uid);
        }
    }

    void EntityManager::ShutDown()
    {}

    HNCRSP_NODISCARD EntityUID EntityManager::CreateEntity()
    {
        if (m_entityUIDCount >= MAX_ENTITIES)
            HNCRSP_TERMINATE("Reached max current entities.");

        EntityUID uid = m_availableEntityUIDs.front();
        m_availableEntityUIDs.pop();
        m_entityUIDCount++;
        return uid;
    }

    void EntityManager::DestroyEntity(EntityUID uid)
    {
        if (uid > MAX_ENTITIES)
            HNCRSP_TERMINATE("Entity out of bounds.");

        m_componentBitsets[uid].reset();  // delete entity's component data
        m_availableEntityUIDs.push(uid);  // push uid back the queue
        --m_entityUIDCount;  // -1 current living entity
    }

    void EntityManager::SetComponentBitset(EntityUID uid, const ComponentBitset& component_bitset)
    {
        if (uid > MAX_ENTITIES)
            HNCRSP_TERMINATE("Entity out of bounds.");

        m_componentBitsets[uid] = component_bitset;
    }

    const ComponentBitset& EntityManager::GetComponentBitset(EntityUID uid)
    {
        if (uid > MAX_ENTITIES)
            HNCRSP_TERMINATE("Entity out of bounds.");

        return m_componentBitsets[uid];
    }
}  // namespace ECS

HNCRSP_NAMESPACE_END