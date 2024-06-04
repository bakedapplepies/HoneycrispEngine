#pragma once

#include "src/pch/pch.h"
#include "System.h"
#include "ComponentCounter.h"
#include "src/utils/binary_insert_delete.h"


HNCRSP_NAMESPACE_START

namespace ECS
{
    class SystemManager
    {
    private:
        std::unordered_map<const char*, ComponentBitset> m_systemComponentBitsets;
        std::unordered_map< const char*, std::unique_ptr<System> > m_systems;

    public:
        SystemManager() = default;
        SystemManager(const SystemManager&) = delete;
        SystemManager& operator=(const SystemManager&) = delete;
        SystemManager(SystemManager&&) noexcept = delete;
        SystemManager& operator=(SystemManager&&) noexcept = delete;
        ~SystemManager() = default;

        void StartUp() {}

        template <typename TSystem>
        [[nodiscard]] TSystem* RegisterSystem(const ComponentBitset& component_bitset)
        {
            static_assert(std::is_base_of_v<System, TSystem>, "TSystem is not a base of 'System'.");

            const char* system_name = typeid(TSystem).name();
            if (m_systemComponentBitsets.find(system_name) != m_systemComponentBitsets.end())
                HNCRSP_TERMINATE("TSystem already registered.");

            m_systemComponentBitsets[system_name] = component_bitset;

            std::unique_ptr<TSystem> newSystem = std::make_unique<TSystem>();
            TSystem* systemRawPtr = newSystem.get();
            m_systems[system_name] = std::move(newSystem);
            return systemRawPtr;
        }

        template <typename TSystem>
        [[nodiscard]] std::shared_ptr<TSystem> GetSystem()
        {
            static_assert(std::is_base_of_v<System, TSystem>, "TSystem is not a base of 'System'.");

            const char* system_name = typeid(TSystem).name();
            if (m_systemComponentBitsets.find(system_name) == m_systemComponentBitsets.end())
                HNCRSP_TERMINATE("TSystem not registered.");

            return std::static_pointer_cast<TSystem>(m_systems[system_name]);
        }

        void EntityComponentBitsetChanged(EntityUID uid, const ComponentBitset& entity_component_bitset);
        void EntityDestroyed(EntityUID uid);
    };
}  // namespace ECS

HNCRSP_NAMESPACE_END