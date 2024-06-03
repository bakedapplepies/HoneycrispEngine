#include "SystemManager.h"
#include "ECSManager.h"
#include "src/managers/SceneManager.h"


HNCRSP_NAMESPACE_START

namespace ECS
{
    void SystemManager::EntityComponentBitsetChanged(EntityUID uid, const ComponentBitset& entity_component_bitset)
    {
        for (const auto& i : m_systemComponentBitsets)  // update all systems to add appropriate EntityUID
        {
            const auto& system_name = i.first;  // str
            const ComponentBitset& system_bitset = i.second;
            System* const system = m_systems[system_name].get();

            std::vector<EntityUID>& systemEUIDs = system->entityUIDs;
            const auto& EUID_iter = std::find(systemEUIDs.begin(), systemEUIDs.end(), uid);
            if ((entity_component_bitset & system_bitset) == system_bitset)
            {
                if (EUID_iter == systemEUIDs.end())
                {
                    system->AddEntityUID(uid);  // unique implementation by each system
                }
            }
            else if (EUID_iter != systemEUIDs.end())
            {
                system->entityUIDs.erase(EUID_iter);
            }
        }
    }

    void SystemManager::EntityDestroyed(EntityUID uid)
    {
        for (const auto& i : m_systems)
        {
            System* system = i.second.get();

            std::vector<EntityUID>& systemEUIDs = system->entityUIDs;
            const auto& EUID_iter = std::find(systemEUIDs.begin(), systemEUIDs.end(), uid);
            if (EUID_iter != systemEUIDs.end())
            {
                system->entityUIDs.erase(EUID_iter);
            }
        }
    }
}  // namespace ECS

HNCRSP_NAMESPACE_END