#pragma once

#include "api/pch/hncrsp_pch.h"
#include "src/ecs/ComponentCounter.h"
#include "src/ecs/EntityManager.h"
#include "src/ecs/ComponentManager.h"
#include "src/ecs/SystemManager.h"

HNCRSP_NAMESPACE_START

class SceneManager;
namespace ECS
{
    class ECSManager
    {
    private:
        EntityManager m_entityManager;
        ComponentManager m_componentManager;
        SystemManager m_systemManager;

    public:
        ECSManager() = default;
        ECSManager(const ECSManager&) = delete;
        ECSManager& operator=(const ECSManager&) = delete;
        ECSManager(ECSManager&& other) = delete;
        ECSManager& operator=(ECSManager&& other) = delete;
        ~ECSManager() = default;

        HNCRSP_NODISCARD uint64_t StartUp();
        void ShutDown(uint64_t key);

        HNCRSP_NODISCARD EntityUID NewEntity();
        void DestroyEntity(EntityUID uid);

        // also registers the component and their bit index
        template <typename TComponent>
        void AddComponent(EntityUID uid, const TComponent& data)
        {
            size_t bit_index = GetBitIndex<TComponent>();
            
            // set new bitset for entity
            ComponentBitset new_component_bitset = m_entityManager.GetComponentBitset(uid);
            new_component_bitset.set(bit_index, 1);
            m_entityManager.SetComponentBitset(uid, new_component_bitset);

            // if entity bitset has all the component bit of a system
            // system will act upon entity's data
            m_componentManager.AddEntityData(uid, data);
            m_systemManager.EntityComponentBitsetChanged(uid, new_component_bitset);
        }

        template <typename TComponent>
        void RemoveComponent(EntityUID uid)
        {
            size_t bit_index = GetBitIndex<TComponent>();

            // set new bitset for entity
            ComponentBitset new_component_bitset = m_entityManager.GetComponentBitset(uid);
            new_component_bitset.set(bit_index, 0);
            m_entityManager.SetComponentBitset(uid, new_component_bitset);

            m_componentManager.RemoveEntityData<TComponent>(uid);
            m_systemManager.EntityComponentBitsetChanged(uid, new_component_bitset);   
        }

        template <typename TComponent>
        HNCRSP_NODISCARD TComponent& GetComponent(EntityUID uid)
        {
            return m_componentManager.GetData<TComponent>(uid);
        }

        template <typename TSystem>
        void RegisterSystem(const ComponentBitset& component_bitset)
        {
            HNCRSP_STATIC_ASSERT(std::is_base_of_v<System, TSystem>, "TSystem is not a base of 'System'.");

            TSystem* system = m_systemManager.RegisterSystem<TSystem>(component_bitset);
        }

        template <typename TSystem>
        HNCRSP_NODISCARD TSystem* GetSystem()
        {
            HNCRSP_STATIC_ASSERT(std::is_base_of_v<System, TSystem>, "TSystem is not a base of 'System'.");

            return m_systemManager.GetSystem<TSystem>();
        }

        template <typename TComponent>
        void RegisterComponent()
        {
            m_componentManager.RegisterComponent<TComponent>();
        }

    private:
        friend SceneManager;
        void _SceneCreated();
        void _SceneChanged(uint32_t current_scene, uint32_t target_scene);
    };
} // namespace ECS

extern ECS::ECSManager g_ECSManager;

HNCRSP_NAMESPACE_END