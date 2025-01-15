#include "api/ecs/SystemManager.h"
#include "api/ecs/ECSManager.h"
#include "api/managers/SceneManager.h"

HNCRSP_NAMESPACE_START

namespace ECS
{
    void SystemManager::EntityComponentBitsetChanged(EntityUID uid, const ComponentBitset& entity_component_bitset)
    {
        for (const auto& i : m_systems)  // Update all systems to add appropriate EntityUID
        {
            const char* system_name = i.first;
            const ComponentBitset& system_bitset = i.second.componentBitset;
            System* const system = m_systems[system_name].system.get();

            std::vector<EntityUID>& systemEUIDs = system->p_entityUIDs;
            const auto& EUID_iter = std::find(systemEUIDs.begin(), systemEUIDs.end(), uid);
            if ((entity_component_bitset & system_bitset) == system_bitset)
            {
                if (EUID_iter == systemEUIDs.end())  // In case ID haven't been added to this system yet
                {
                    system->AddEntityUID(uid);  // Unique implementation by each system
                }
            }
            else if (EUID_iter != systemEUIDs.end())  // If ID bitset doesn't match and it's still in the system, delete it
            {
                system->p_entityUIDs.erase(EUID_iter);
            }
        }
    }

    void SystemManager::EntityDestroyed(EntityUID uid)
    {
        for (const auto& i : m_systems)  // Delete entity ID from all systems
        {
            System* system = i.second.system.get();

            std::vector<EntityUID>& systemEUIDs = system->p_entityUIDs;
            const auto& EUID_iter = std::find(systemEUIDs.begin(), systemEUIDs.end(), uid);
            if (EUID_iter != systemEUIDs.end())
            {
                system->p_entityUIDs.erase(EUID_iter);
            }
        }
    }

    void SystemManager::SceneCreated()
    {
        for (auto& i : m_systems)
        {
            i.second.entityListPerScene.push_back({});  // Create new empty EntityUID list for new scene
        }
    }

    void SystemManager::SceneChanged(uint32_t current_scene, uint32_t target_scene)
    {
        for (auto& i : m_systems)
        {
            System* system = i.second.system.get();
            std::swap(system->p_entityUIDs, i.second.entityListPerScene[current_scene]);  // store the current entity list away
            std::swap(system->p_entityUIDs, i.second.entityListPerScene[target_scene]);  // swap for new entity list

            system->SceneChanged(target_scene);
        }
    }
}  // namespace ECS

HNCRSP_NAMESPACE_END