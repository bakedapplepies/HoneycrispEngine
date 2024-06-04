#pragma once

#include "src/pch/pch.h"
#include "ComponentCounter.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "src/systems/Renderer.h"


HNCRSP_NAMESPACE_START

namespace ECS
{
    struct TimeBySystems
    {  // Note: float is capable of storing up to ~3.4E+38
        RendererTime renderer;
    };

    class ECSManager
    {
    private:
        std::unique_ptr<EntityManager> m_entityManager;
        std::unique_ptr<ComponentManager> m_componentManager;
        std::unique_ptr<SystemManager> m_systemManager;

        // Systems are set by the template functions below
        Renderer* m_renderer;
        mutable TimeBySystems m_timeBySystems;  // TODO: Use this to update GUI

    public:
        ECSManager() = default;
        ECSManager(const ECSManager&) = delete;
        ECSManager& operator=(const ECSManager&) = delete;
        ECSManager(ECSManager&& other) noexcept;
        ECSManager& operator=(ECSManager&& other) noexcept;
        ~ECSManager() = default;

        void StartUp();
        void ShutDown();
        void Update() const;
        void Renderer_SetCubemap(std::weak_ptr<Cubemap> weak_cubemap);
        const TimeBySystems* GetTimeBySystems() const;

        EntityUID NewEntityUID() const;
        void DestroyEntity(EntityUID uid) const;

        // also registers the component and their bit index
        template <typename TComponent>
        void AddComponent(EntityUID uid, const TComponent& data)
        {
            size_t bit_index = GetBitIndex<TComponent>();
            
            // set new bitset for entity
            ComponentBitset new_component_bitset = m_entityManager->GetComponentBitset(uid);
            new_component_bitset.set(bit_index, 1);
            m_entityManager->SetComponentBitset(uid, new_component_bitset);

            // if entity bitset has all the component bit of a system
            // system will act upon entity's data
            m_componentManager->AddEntityData(uid, data);
            m_systemManager->EntityComponentBitsetChanged(uid, new_component_bitset);
        }

        template <typename TComponent>
        void RemoveComponent(EntityUID uid)
        {
            size_t bit_index = GetBitIndex<TComponent>();

            // set new bitset for entity
            ComponentBitset new_component_bitset = m_entityManager->GetComponentBitset(uid);
            new_component_bitset.set(bit_index, 0);
            m_entityManager->SetComponentBitset(uid, new_component_bitset);

            m_componentManager->RemoveEntityData<TComponent>(uid);
            m_systemManager->EntityComponentBitsetChanged(uid, new_component_bitset);   
        }

        template <typename TComponent>
        [[nodiscard]] TComponent& GetComponent(EntityUID uid)
        {
            return m_componentManager->GetData<TComponent>(uid);
        }

        template <typename TSystem>
        void RegisterSystem(const ComponentBitset& component_bitset)
        {
            static_assert(std::is_base_of_v<System, TSystem>, "TSystem is not a base of 'System'.");

            TSystem* system = m_systemManager->RegisterSystem<TSystem>(component_bitset);

            // TODO: Need some rework here, this is ugly
            if constexpr(std::is_same_v<TSystem, Renderer>) m_renderer = system;
        }

        template <typename TSystem>
        [[nodiscard]] std::shared_ptr<TSystem> GetSystem()
        {
            static_assert(std::is_base_of_v<System, TSystem>, "TSystem is not a base of 'System'.");

            return m_systemManager->GetSystem<TSystem>();
        }

        template <typename TComponent>
        void RegisterComponent() const
        {
            m_componentManager->RegisterComponent<TComponent>();
        }
    };
} // namespace ECS

extern ECS::ECSManager* g_ECSManager;

HNCRSP_NAMESPACE_END