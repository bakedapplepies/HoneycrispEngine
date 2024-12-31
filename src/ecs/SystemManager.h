#pragma once

#include "src/pch/hncrsp_pch.h"
#include "System.h"
#include "ComponentCounter.h"
#include "src/utils/binary_insert_delete.h"


HNCRSP_NAMESPACE_START

namespace ECS
{
    struct SystemAndProperties
    {
        std::unique_ptr<System> system;
        ComponentBitset componentBitset;
        std::vector< std::vector<EntityUID> > entityListPerScene;
    };

    class SystemManager
    {
    private:
        std::unordered_map<const char*, SystemAndProperties> m_systems;

    public:
        SystemManager() = default;
        SystemManager(const SystemManager&) = delete;
        SystemManager& operator=(const SystemManager&) = delete;
        SystemManager(SystemManager&&) noexcept = delete;
        SystemManager& operator=(SystemManager&&) noexcept = delete;
        ~SystemManager() = default;

        inline void StartUp()
        {}

        inline void ShutDown()
        {
            m_systems.clear();
        }

        template <typename TSystem>
        HNCRSP_NODISCARD TSystem* RegisterSystem(const ComponentBitset& component_bitset)
        {
            HNCRSP_STATIC_ASSERT(std::is_base_of_v<System, TSystem>, "TSystem is not a base of 'System'.");

            const char* system_name = typeid(TSystem).name();
            if (m_systems.find(system_name) != m_systems.end())
                HNCRSP_TERMINATE("TSystem already registered.");

            m_systems[system_name].componentBitset = component_bitset;

            std::unique_ptr<TSystem> newSystem = std::make_unique<TSystem>();
            TSystem* systemRawPtr = newSystem.get();
            m_systems[system_name].system = std::move(newSystem);
            return systemRawPtr;
        }

        template <typename TSystem>
        HNCRSP_NODISCARD TSystem* const GetSystem()
        {
            HNCRSP_STATIC_ASSERT(std::is_base_of_v<System, TSystem>, "TSystem is not a base of 'System'.");

            const char* system_name = typeid(TSystem).name();
            if (m_systems.find(system_name) == m_systems.end())
                HNCRSP_TERMINATE("TSystem not registered.");

            return static_cast<TSystem*>(m_systems[system_name].system.get());
        }

        void EntityComponentBitsetChanged(EntityUID uid, const ComponentBitset& entity_component_bitset);
        void EntityDestroyed(EntityUID uid);
        void SceneCreated();
        void SceneChanged(uint32_t current_scene, uint32_t target_scene);
    };
}  // namespace ECS

HNCRSP_NAMESPACE_END