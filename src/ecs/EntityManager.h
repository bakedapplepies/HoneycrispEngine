#pragma once

#include "src/pch/hncrsp_pch.h"
#include "ECSTypes.h"


HNCRSP_NAMESPACE_START

namespace ECS
{
    class EntityManager
    {
    private:
        std::queue<EntityUID> m_availableEntityUIDs;
        std::array<ComponentBitset, MAX_ENTITIES> m_componentBitsets;
        EntityUID m_entityUIDCount = 0;

    public:
        EntityManager() = default;
        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;
        EntityManager(EntityManager&&) noexcept = delete;
        EntityManager& operator=(EntityManager&&) noexcept = delete;
        ~EntityManager() = default;
        
        void StartUp();  // easier control over managers
        EntityUID CreateEntity();
        void DestroyEntity(EntityUID uid);
        void SetComponentBitset(EntityUID uid, const ComponentBitset& component_bitset);
        const ComponentBitset& GetComponentBitset(EntityUID uid);
    };
}  // namespace ECS

HNCRSP_NAMESPACE_END