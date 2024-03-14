#include "ECSManager.h"


HNCRSP_NAMESPACE_START

ECSManager* g_ECSManager = nullptr;

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
    m_renderer->Render();
}

void ECSManager::ShutDown()
{
    //  I still like the ownership idea of not sharing pointers,
    // so I'm keeping these as unique_ptrs
    m_entityManager.reset();
    m_componentManager.reset();
    m_systemManager.reset();
}

[[nodiscard]] EntityUID ECSManager::NewEntityUID() const
{
    // TODO: Renderer can reorganize the render queue here
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

HNCRSP_NAMESPACE_END