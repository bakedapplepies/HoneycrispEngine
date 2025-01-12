#pragma once

#include "api/pch/hncrsp_pch.h"
#include "ComponentArray.h"


HNCRSP_NAMESPACE_START

namespace ECS
{
    using ComponentBit = std::uint8_t;

    class ComponentManager
    {
    private:
        // std::unordered_map<const char*, ComponentBit> m_componentsBits;
        std::unordered_map< const char*, std::shared_ptr<IComponentArray> > m_componentArrays;

    private:
        template <typename TComponent>
        std::shared_ptr< ComponentArray<TComponent> > GetComponentArray()
        {
            const char* component_name = typeid(TComponent).name();
            if (m_componentArrays.find(component_name) == m_componentArrays.end())
                HNCRSP_TERMINATE("TComponent has never been used.");

            return std::static_pointer_cast<ComponentArray<TComponent>>(m_componentArrays[component_name]);
        }

    public:
        ComponentManager() = default;
        ComponentManager(const ComponentManager&) = delete;
        ComponentManager& operator=(const ComponentManager&) = delete;
        ComponentManager(ComponentManager&&) noexcept = delete;
        ComponentManager& operator=(ComponentManager&&) noexcept = delete;
        ~ComponentManager() = default;

        inline void StartUp()
        {}

        inline void ShutDown()
        {
            m_componentArrays.clear();
        }

        template <typename TComponent>
        void RegisterComponent()
        {
            const char* component_name = typeid(TComponent).name();
            if (m_componentArrays.find(component_name) != m_componentArrays.end())
                HNCRSP_TERMINATE("TComponent already registered.");

            m_componentArrays[component_name] = std::make_shared<ComponentArray<TComponent>>();
        }

        template <typename TComponent>
        void AddEntityData(EntityUID uid, const TComponent& data)
        {
            const char* component_name = typeid(TComponent).name();
            if (m_componentArrays.find(component_name) == m_componentArrays.end())
                HNCRSP_TERMINATE("TComponent not registered.");

            GetComponentArray<TComponent>()->InsertData(uid, data);
        }

        template <typename TComponent>
        void RemoveEntityData(EntityUID uid)
        {
            const char* component_name = typeid(TComponent).name();
            if (m_componentArrays.find(component_name) == m_componentArrays.end())
                HNCRSP_TERMINATE("TComponent not registered.");

            GetComponentArray<TComponent>()->RemoveData(uid);
        }

        template <typename TComponent>
        TComponent& GetData(EntityUID uid)
        {
            const char* component_name = typeid(TComponent).name();
            if (m_componentArrays.find(component_name) == m_componentArrays.end())
                HNCRSP_TERMINATE("TComponent not registered.");
            
            return GetComponentArray<TComponent>()->GetData(uid);
        }

        inline void EntityDestroyed(EntityUID uid)
        {
            for (const auto& pair : m_componentArrays)
            {
                const auto& componentArray = pair.second;
                componentArray->EntityDestroyed(uid);
            }
        }
    };
}  // namespace ECS

HNCRSP_NAMESPACE_END