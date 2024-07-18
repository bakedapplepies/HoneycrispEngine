#pragma once

#include "src/pch/hncrsp_pch.h"
#include "ECSTypes.h"


HNCRSP_NAMESPACE_START

namespace ECS
{
    class SystemManager;
    class System
    {
    protected:
        friend SystemManager;

        // TODO: Make a vector-like interface to communicate with
        // System Manager, which leaves the System free to store its
        // data however it likes.
        std::vector<EntityUID> p_entityUIDs;

        // Since Renderer has a unique way of storing entities to
        // avoid state changes, might as well make all system do this
        // in case they want to optimize how they store things.
        virtual void AddEntityUID(EntityUID entity_UID) = 0;

    public:
        virtual ~System() = default;
    };
}  // namespace ECS

HNCRSP_NAMESPACE_END