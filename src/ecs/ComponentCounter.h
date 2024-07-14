#pragma once

#include "src/pch/hncrsp_pch.h"
#include "ComponentArray.h"


HNCRSP_NAMESPACE_START

namespace ECS
{
    inline size_t& ComponentCounter()
    {
        static size_t counter = 0;
        return counter;
    }

    template <typename T>
    struct Helper
    {
        inline static size_t bit_index = [] {
            return ComponentCounter()++;
        }();
    };

    template <auto>
    struct Use {};

    template <typename TComponent>
    size_t GetBitIndex()
    {
        Use<&Helper<TComponent>::bit_index> _;

        if (ComponentCounter() > MAX_COMPONENTS)
        {
            HNCRSP_TERMINATE("Max components reached.");
        }

        return Helper<TComponent>::bit_index;
    }
}  // namespace ECS

HNCRSP_NAMESPACE_END