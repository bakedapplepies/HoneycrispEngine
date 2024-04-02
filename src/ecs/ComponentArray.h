#pragma once

#include "src/pch/pch.h"
#include "src/ecs/ECSTypes.h"
#include "src/components/MeshData.h"


HNCRSP_NAMESPACE_START

class IComponentArray  // ComponentArray Interface
{
public:
    virtual void EntityDestroyed(EntityUID uid) = 0;
    virtual ~IComponentArray() = default;
};

template<typename TComponent>
class ComponentArray : public IComponentArray
{
private:
    // each template instantiation -> one component type for all entities
    std::array<TComponent, MAX_ENTITIES> m_componentArray;

    // we need to arrange the index so that it points to contiguous memory
    // then the EntityUID can just point to that index
    std::unordered_map<EntityUID, size_t> m_entityUIDToIndexMap;
    std::unordered_map<size_t, EntityUID> m_indexToEntityUIDMap;

    // Since the ComponentArray is always packed, m_nextFinalIndex acts as the next index
    size_t m_nextFinalIndex = 0;

public:
    void InsertData(EntityUID uid, const TComponent& componentData)
    {
        if (m_entityUIDToIndexMap.find(uid) != m_entityUIDToIndexMap.end())
            HNCRSP_TERMINATE(fmt::format("{} already added to entity.", typeid(TComponent).name()).c_str());

        size_t newIndex = m_nextFinalIndex;
        m_entityUIDToIndexMap[uid] = newIndex;
        m_indexToEntityUIDMap[newIndex] = uid;
        m_componentArray[newIndex] = componentData;
        m_nextFinalIndex++;
    }

    void RemoveData(EntityUID uid)
    {
        if (m_entityUIDToIndexMap.find(uid) == m_entityUIDToIndexMap.end())
            HNCRSP_TERMINATE(fmt::format("{} not yet added to entity.", typeid(TComponent).name()).c_str());

        size_t indexOfRemoveData = m_entityUIDToIndexMap[uid];
        EntityUID lastEntityUID = m_indexToEntityUIDMap[m_nextFinalIndex - 1];
        
        m_componentArray[indexOfRemoveData] = m_componentArray[m_nextFinalIndex - 1];
        m_entityUIDToIndexMap[lastEntityUID] = indexOfRemoveData;
        m_indexToEntityUIDMap[indexOfRemoveData] = lastEntityUID;

        m_entityUIDToIndexMap.erase(uid);  // delete uid after switching things up
        m_indexToEntityUIDMap.erase(m_nextFinalIndex - 1);  // always delete the last element

        m_nextFinalIndex--;
    }

    TComponent& GetData(EntityUID uid)
    {
        if (m_entityUIDToIndexMap.find(uid) == m_entityUIDToIndexMap.end())
            HNCRSP_TERMINATE(fmt::format("{} not yet added to entity.", typeid(TComponent).name()).c_str());

        return m_componentArray[m_entityUIDToIndexMap[uid]];
    }

    // Is called when an entity is destroyed to delete all components.
    // If entity doesn't have component, component doesn't get deleted.
    void EntityDestroyed(EntityUID uid) override
    {
        if (m_entityUIDToIndexMap.find(uid) != m_entityUIDToIndexMap.end())
        {
            RemoveData(uid);
        }
    }
};

HNCRSP_NAMESPACE_END