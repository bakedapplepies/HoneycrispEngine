#include "SystemManager.h"
#include "ECSManager.h"
#include "src/core/SceneManager.h"


HNCRSP_NAMESPACE_START

void SystemManager::EntityComponentBitsetChanged(EntityUID uid, const ComponentBitset& entity_component_bitset)
{
    for (const auto& i : m_systemComponentBitsets)  // update all systems to add appropriate entity
    {
        const auto& system_name = i.first;  // str
        const ComponentBitset& system_bitset = i.second;
        System* const system = m_systems[system_name].get();

        std::vector<EntityUID>& systemEUIDs = system->entityUIDs;
        const auto& EUID_iter = std::find(systemEUIDs.begin(), systemEUIDs.end(), uid);
        if ((entity_component_bitset & system_bitset) == system_bitset)
        {
            // we don't have to check if entity is already added to system since
            // it's removed at all times except when its component_bitset matches
            // the system
            ComponentBitset meshDataBitset = ComponentBitset().set(GetBitIndex<MeshData>());
            if ((entity_component_bitset & meshDataBitset) == meshDataBitset)
            {
                GLuint shaderID = g_ECSManager->GetComponent<MeshData>(uid).material->getShader()->getID();
                binary_insert_shader_comparator(system->entityUIDs, m_shaderIDs_Order, uid, shaderID);
            }
            else
            {
                system->entityUIDs.push_back(uid);
            }
        }
        else
        {
            if (EUID_iter != systemEUIDs.end())
            {
                system->entityUIDs.erase(EUID_iter);
            }
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

HNCRSP_NAMESPACE_END