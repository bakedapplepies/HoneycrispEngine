#include "ECSManager.h"
#include "src/utils/TracyProfile.h"


HNCRSP_NAMESPACE_START

ECS::ECSManager* g_ECSManager = nullptr;

namespace ECS
{
    void ECSManager::StartUp()
    {
        m_entityManager = std::make_unique<EntityManager>();
        m_entityManager->StartUp();
        m_componentManager = std::make_unique<ComponentManager>();
        m_componentManager->StartUp();
        m_systemManager = std::make_unique<SystemManager>();
        m_systemManager->StartUp();
    }

    void ECSManager::Update() const
    {
        // ZoneScoped;
        m_renderer->Render(&m_timeBySystems.renderer);
        // FrameMark;
    }

    void ECSManager::ShutDown()
    {
        m_entityManager.reset();
        m_componentManager.reset();
        m_systemManager.reset();
    }

    void ECSManager::Renderer_SetCubemap(std::weak_ptr<Cubemap> weak_cubemap)
    {
        m_renderer->SwitchCubemap(weak_cubemap);
    }

    const TimeBySystems* ECSManager::GetTimeBySystems() const
    {
        return &m_timeBySystems;
    }

    [[nodiscard]] EntityUID ECSManager::NewEntityUID() const
    {
        return m_entityManager->CreateEntity();
    }

    void ECSManager::DestroyEntity(EntityUID uid) const
    {
        // put uid back in queue
        m_entityManager->DestroyEntity(uid);

        // delete entity's data and keep data packed
        m_componentManager->EntityDestroyed(uid);

        // delete entity uid off system
        m_systemManager->EntityDestroyed(uid);
    }

    ECSManager::ECSManager(ECSManager&& other) noexcept
    {
        m_entityManager = std::move(other.m_entityManager);
        m_componentManager = std::move(other.m_componentManager);
        m_systemManager = std::move(other.m_systemManager);
    }

    ECSManager& ECSManager::operator=(ECSManager&& other) noexcept
    {
        m_entityManager = std::move(other.m_entityManager);
        m_componentManager = std::move(other.m_componentManager);
        m_systemManager = std::move(other.m_systemManager);

        return *this;
    }
}  // namespace ECS

HNCRSP_NAMESPACE_END